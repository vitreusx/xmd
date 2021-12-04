#include "dynamics/lang_leapfrog.h"
#include <xmd/utils/units.h>

namespace xmd {
    void lang_leapfrog_step::operator()() const {
        auto local_gen = *gen;
        real noise_factor = sqrt(2.0 * kB * temperature);

        for (int idx = 0; idx < num_particles; ++idx) {
            auto gamma = gamma_factor * mass[idx];
            auto noise_sd = noise_factor * sqrt(gamma);

            auto [noise_x, noise_y] = local_gen.normalx2<real>();
            auto noise_z = local_gen.normal<real>();
            auto noise = vec3r(noise_x, noise_y, noise_z);

            vec3tr vel_invariant_term = mass_inv[idx] * (F[idx] + noise_sd * noise);
            vec3tr cur_v = (true_prev_v[idx] + 0.5 * (prev_a[idx] -
                vel_invariant_term * dt)) / (1.0 + 0.5 * mass_inv[idx] *
                    gamma * dt);
            vec3tr cur_a = vel_invariant_term - mass_inv[idx] * cur_v;
            vec3tr next_r = r[idx] + (cur_v + 0.5 * cur_a * dt) * dt;

            r[idx] = true_r[idx] = next_r;
            prev_v[idx] = true_prev_v[idx] = cur_v;
            prev_a[idx] = cur_a;
        }

        *t = (*true_t += dt);
        *gen = local_gen;
    }
}