#include "dynamics/reset_vf.h"

namespace xmd {
    void reset_vf::operator()() const {
        *V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3r::Zero();
        }
    }

    void reset_vf::declare_vars(context& ctx) {
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");
        num_particles = ctx.var<int>("num_particles");
    }

    void reset_vf::omp_async() const {
        *V = 0.0f;

#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3r::Zero();
        }
    }
}