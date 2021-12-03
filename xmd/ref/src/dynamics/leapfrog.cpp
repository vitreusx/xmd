#include "dynamics/leapfrog.h"

namespace xmd {
    void perform_leapfrog_step::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx) {
            auto a_cur = F[idx] * mass_inv[idx];
            true_v[idx] += 0.5 * (a_prev[idx] + a_cur) * dt;
            true_r[idx] += true_v[idx] * dt + 0.5 * a_cur * dt * dt;
            a_prev[idx] = a_cur;
            r[idx] = true_r[idx];
            v[idx] = true_v[idx];
        }

        *true_t += dt;
        *t = (float)*true_t;
    }
}