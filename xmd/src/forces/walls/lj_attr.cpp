#include "forces/walls/lj_attr.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    void eval_lj_attr_wall_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        wall_min_dist = ctx.persistent<real>("lj_attr_wall_min_dist",
            params["LJ attractive walls"]["wall min dist"].as<quantity>());
        breaking_factor = ctx.persistent<real>("lj_attr_breaking_factor",
            params["LJ attractive walls"]["breaking factor"].as<quantity>());
        factor = pow(2.0, -1.0/6.0) * breaking_factor;
        cycle_time = ctx.persistent<real>("lj_attr_cycle_time",
            params["LJ attractive walls"]["cycle time"].as<quantity>());
        cycle_time_inv = 1.0 / cycle_time;

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");
        box = &ctx.var<xmd::box>("box");
        num_particles = ctx.var<int>("num_particles");
        t = &ctx.var<real>("r");

        pairs = ctx.persistent<vector<lj_attr_pair>>("lj_attr_pairs", lazy([&]() -> auto {
            vector<lj_attr_pair> pairs_;

            for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
                    pairs_.emplace_back(part_idx, wall_idx, FREE,
                        vec3r::Zero(),0.0f);
                }
            }

            return pairs_;
        })).view();

        walls = ctx.persistent<vector<plane>>("lj_attr_walls", lazy([&]() -> auto {
            vector<plane> walls_;

            for (auto const& plane_node: params["LJ attractive walls"]["planes"]) {
                auto origin = plane_node["origin"].as<vec3r>();
                auto normal = plane_node["normal"].as<vec3r>();
                walls_.emplace_back(origin, normal);
            }

            return walls_;
        })).view();

        wall_F = ctx.persistent<vector<vec3r>>("lj_attr_wall_F",
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