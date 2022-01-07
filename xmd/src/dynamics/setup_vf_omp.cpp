#include "dynamics/setup_vf_omp.h"

namespace xmd {
    void setup_vf_omp::declare_vars(context& ctx) {
        num_particles = ctx.var<int>("num_particles");

        auto &shared_F = ctx.var<vector<vec3r>>("F");
        ctx.persistent<vector<vec3r>*>("shared_F_ptr", &shared_F);
        thread_F = ctx.persistent<vector<vec3r>>("F", num_particles).data();

        auto &shared_V = ctx.var<real>("V");
        ctx.ephemeral<real*>("shared_V_ptr", &shared_V);
        thread_V = &ctx.persistent<real>("V", (real)0.0);
    }

    void setup_vf_omp::operator()() const {
        *thread_V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            thread_F[idx] = vec3r::Zero();
        }
    }
}