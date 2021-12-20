#include "forces/walls/lj_attr.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    lj_attr_pairs_vector::lj_attr_pairs_vector(int n):
        part_idx{n}, wall_idx{n}, status{n}, joint_r{n}, ref_t{n}, size{n} {};

    lj_attr_pairs_span lj_attr_pairs_vector::to_span() {
        lj_attr_pairs_span s;
        s.part_idx = part_idx.to_array();
        s.wall_idx = wall_idx.to_array();
        s.joint_r = joint_r.to_array();
        s.ref_t = ref_t.to_array();
        s.size = size;
        return s;
    }

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

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        num_particles = vm_inst.find<int>("num_particles");
        t = &vm_inst.find<real>("r");

        pairs = vm_inst.find_or<lj_attr_pairs_vector>("lj_attr_pairs", [&]() -> auto& {
            auto num_pairs = num_particles * walls.size();
            auto& pairs_ = vm_inst.emplace<lj_attr_pairs_vector>("lj_attr_pairs",
                num_pairs);

            auto pair_idx = 0;
            for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
                    pairs_.part_idx[pair_idx] = part_idx;
                    pairs_.wall_idx[pair_idx] = wall_idx;
                    pairs_.status[pair_idx] = FREE;
                    ++pair_idx;
                }
            }

            return pairs_;
        }).to_span();

        walls = vm_inst.find_or<vector<plane>>("lj_attr_walls", [&]() -> auto& {
            auto& walls_ = vm_inst.emplace<vector<plane>>("lj_attr_walls");
            for (auto const& plane_node: params["LJ attractive walls"]["planes"]) {
                plane p;
                p.normal = plane_node["normal"].as<vec3r>();
                p.origin = plane_node["origin"].as<vec3r>();
                walls_.push_back(p);
            }
            return walls_;
        }).to_span();

        wall_F = vm_inst.find_or_emplace<vec3r_vector>("lj_attr_wall_F",
            walls.size()).to_array();
    }

    void eval_lj_attr_wall_forces::loop(int idx) const {
        auto part_idx = pairs.part_idx[idx], wall_idx = pairs.wall_idx[idx];
        auto status = pairs.status[idx];
        auto r_ = box->warp_to_box(r[part_idx]);
        auto wall = walls[wall_idx];

        if (status == FREE) {
            if (abs(signed_dist(r_, wall)) < wall_min_dist) {
                pairs.status[idx] = FORMING_OR_FORMED;
                pairs.joint_r[idx] = project(r_, wall);
                pairs.ref_t[idx] = *t;
            }
        }
        else {
            auto joint_r = pairs.joint_r[idx];
            auto ref_t = pairs.ref_t[idx];
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
                    pairs.status[idx] = BREAKING;
                    pairs.ref_t[idx] = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
                pairs.status[idx] = FREE;
            }
        }
    }

    void eval_lj_attr_wall_forces::operator()() const {
        for (int idx = 0; idx < pairs.size; ++idx) {
            loop(idx);
        }
    }

    void eval_lj_attr_wall_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < pairs.size; ++idx) {
            loop(idx);
        }
    }
}