#include "dynamics/reduce_vf.h"

namespace xmd {
    void reduce_vf::declare_vars(context& ctx) {
        num_particles = ctx.var<int>("num_particles");
        F = ctx.var<vector<vec3r>>("F").data();
        thread_F = ctx.per_thread().var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");
        thread_V = &ctx.per_thread().var<real>("V");
    }

    void reduce_vf::operator()() const {
#pragma omp atomic update
        *V += *thread_V;

        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx].atomic_add(thread_F[idx]);
        }
    }
}