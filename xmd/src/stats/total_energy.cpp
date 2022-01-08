#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        real K_ = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            K_ += (real)0.5 * mass[idx] * norm_squared(v[idx]);
        }
        *K = K_;
        *E = *V + K_;
    }

    void compute_total_energy::declare_vars(context& ctx) {
        v = ctx.var<vector<vec3r>>("r").data();
        mass = ctx.var<vector<real>>("mass").data();
        num_particles = ctx.var<int>("num_particles");
        V = &ctx.per_thread().var<real>("V");
        K = &ctx.persistent<real>("K", (real)0.0);
        E = &ctx.persistent<real>("E", (real)0.0);
    }
}