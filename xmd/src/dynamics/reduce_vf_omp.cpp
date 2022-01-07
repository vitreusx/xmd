#include "dynamics/reduce_vf_omp.h"

namespace xmd {
    void reduce_vf_omp::declare_vars(context& ctx) {
        num_particles = ctx.var<int>("num_particles");
        shared_F = ctx.var<vector<vec3r>*>("shared_F_ptr")->data();
        thread_F = ctx.var<vector<vec3r>>("F").data();
        shared_V = ctx.var<real*>("shared_V_ptr");
        thread_V = &ctx.var<real>("V");
    }

    void reduce_vf_omp::operator()() const {
#pragma omp atomic update
        *shared_V += *thread_V;

        for (int idx = 0; idx < num_particles; ++idx) {
            shared_F[idx].atomic_add(thread_F[idx]);
        }
    }
}