#include "dynamics/reset_thread_vf.h"

namespace xmd {
    void reset_thread_vf::declare_vars(context& ctx) {
        auto& num_particles = ctx.var<int>("num_particles");
        thread_F = ctx.per_thread().ephemeral<vector<vec3r>>("F", num_particles).view();
        thread_V = &ctx.per_thread().ephemeral<real>("V", 0.0f);
    }

    void reset_thread_vf::operator()() const {
        *thread_V = (real)0.0;
        for (int idx = 0; idx < thread_F.size(); ++idx)
            thread_F[idx] = vec3r::Zero();
    }
}