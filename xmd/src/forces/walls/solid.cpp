#include "forces/walls/solid.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_solid_wall_forces::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        eps = ctx.persistent<real>("solid_wall_eps",
            params["solid walls"]["depth"].as<quantity>());
        cutoff = ctx.persistent<real>("solid_wall_cutoff",
            params["solid walls"]["cutoff"].as<quantity>());

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");
        num_particles = ctx.var<int>("num_particles");
        box = &ctx.var<xmd::box>("box");

        walls = ctx.persistent<vector<plane>>("solid_walls", lazy([&]() -> auto {
            vector<plane> walls_;

            for (auto const& plane_node: params["solid walls"]["planes"]) {
                auto origin = plane_node["origin"].as<vec3r>();
                auto normal = plane_node["normal"].as<vec3r>();
                walls_.emplace_back(origin, normal);
            }

            return walls_;
        })).view();

        wall_F = ctx.persistent<vector<vec3r>>("solid_wall_F",
            walls.size()).data();
    }

    void eval_solid_wall_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::iter(int idx) const {
        auto part_r = box->wrap(r[idx]);
        for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
            auto wall = walls[wall_idx];
            auto x = signed_dist(part_r, wall);
            if (x < cutoff) {
                auto V_ = eps / ((real)9.0 * ipow<9>(x));
                auto dV_dx = V_ * ((real)(-9.0) * x);
                *V += V_;
                F[idx] += dV_dx * wall.normal();
                wall_F[wall_idx] -= dV_dx * wall.normal();
            }
        }
    }
}