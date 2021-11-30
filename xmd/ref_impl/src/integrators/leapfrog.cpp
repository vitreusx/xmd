#include "integrators/leapfrog.h"

namespace xmd {
    void perform_leapfrog_step::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx) {
            auto a_cur = F[idx] * mass_inv[idx];
            v[idx] += 0.5f * (a_prev[idx] + a_cur) * dt;
            r[idx] += v[idx] * dt + 0.5f * a_cur * dt * dt;
            a_prev[idx] = a_cur;
        }
        *t += dt;
    }
}