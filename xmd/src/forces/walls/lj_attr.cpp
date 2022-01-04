#include "forces/walls/lj_attr.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    void eval_lj_attr_wall_forces::init_from_vm(vm& vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        wall_min_dist = vm_inst.find_or_emplace<real>("lj_attr_wall_min_dist",
            params["LJ attractive walls"]["wall min dist"].as<quantity>());
        breaking_factor = vm_inst.find_or_emplace<real>("lj_attr_breaking_factor",
            params["LJ attractive walls"]["breaking factor"].as<quantity>());
        factor = pow(2.0, -1.0/6.0) * breaking_factor;
        cycle_time = vm_inst.find_or_emplace<real>("lj_attr_cycle_time",
            params["LJ attractive walls"]["cycle time"].as<quantity>());
        cycle_time_inv = 1.0 / cycle_time;

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box>("box");
        num_particles = vm_inst.find<int>("num_particles");
        t = &vm_inst.find<real>("r");

        pairs = vm_inst.find_or<vector<lj_attr_pair>>("lj_attr_pairs", [&]() -> auto& {
            auto& pairs_ = vm_inst.emplace<vector<lj_attr_pair>>("lj_attr_pairs");

            for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
                    pairs_.emplace_back(part_idx, wall_idx, FREE,
                        vec3r::Zero(),0.0f);
                }
            }

            return pairs_;
        }).view();

        walls = vm_inst.find_or<vector<plane>>("lj_attr_walls", [&]() -> auto& {
            auto& walls_ = vm_inst.emplace<vector<plane>>("lj_attr_walls");
            for (auto const& plane_node: params["LJ attractive walls"]["planes"]) {
                plane p;
                p.normal = plane_node["normal"].as<vec3r>();
                p.origin = plane_node["origin"].as<vec3r>();
                walls_.push_back(p);
            }
            return walls_;
        }).view();

        wall_F = vm_inst.find_or_emplace<vector<vec3r>>("lj_attr_wall_F",
            walls.size()).data();
    }

    void eval_lj_attr_wall_forces::loop(int idx) const {
        auto pair = pairs[idx];

        auto part_idx = pair.part_idx(), wall_idx = pair.wall_idx();
        auto status = pair.status();
        auto r_ = box->wrap(r[part_idx]);
        auto wall = walls[wall_idx];

        if (status == FREE) {
            if (abs(signed_dist(r_, wall)) < wall_min_dist) {
                pair.status() = FORMING_OR_FORMED;
                pair.joint_r() = project(r_, wall);
                pair.ref_t() = *t;
            }
        }
        else {
            auto joint_r = pair.joint_r();
            auto ref_t = pair.ref_t();
            auto saturation = min(*t - ref_t, cycle_time) * cycle_time_inv;
            if (status == BREAKING)
                saturation = (real)1.0 - saturation;

            auto r12 = joint_r - r_;
            auto r12_rn = norm_inv(r12);
            auto [V_, dV_dr] = lj(lj_depth, wall_min_dist)(r12_rn);
            *V += V_ * saturation;

            auto r12_u = r12 * r12_rn;
            auto f = saturation * dV_dr * r12_u;
            F[part_idx] += f;
            wall_F[wall_idx] -= f;

            if (status == FORMING_OR_FORMED && saturation == 1.0f) {
                if (factor * wall_min_dist * r12_rn < 1.0f) {
                    pair.status() = BREAKING;
                    pair.ref_t() = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
                pair.status() = FREE;
            }
        }
    }

    void eval_lj_attr_wall_forces::operator()() const {
        for (int idx = 0; idx < pairs.size(); ++idx) {
            loop(idx);
        }
    }

    void eval_lj_attr_wall_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < pairs.size(); ++idx) {
            loop(idx);
        }
    }
}