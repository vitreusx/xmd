#include "dynamics/lang_pc.h"
#include <xmd/utils/units.h>

namespace xmd {
    void lang_pc_step::operator()() const {
        auto local_gen = *gen;
        real noise_factor = sqrt(2.0 * kB * temperature);
        true_real dt_inv = 1.0/dt;
        auto gamma_factor_sqrt = sqrt(gamma_factor);

        for (int idx = 0; idx < num_particles; ++idx) {
            auto gamma = gamma_factor * mass[idx];
            auto noise_sd = noise_factor * gamma_factor_sqrt * mass_rsqrt[idx] ;

            auto [noise_x, noise_y] = local_gen.normal_x2<real>();
            auto noise_z = local_gen.normal<real>();
            auto noise = vec3r(noise_x, noise_y, noise_z);

            auto a_ = F[idx] * mass_inv[idx] - gamma * v[idx] + noise_sd * noise;

            vec3tr error = y2[idx] - a_ * (dt*dt/2.0);
            y0[idx] -= 3.0/16.0 * error;
            y1[idx] -= 251.0/360.0 * error;
            y2[idx] -= 1.0 * error;
            y3[idx] -= 11.0/18.0 * error;
            y4[idx] -= 1.0/6.0 * error;
            y5[idx] -= 1.0/60.0 * error;

            y0[idx] += y1[idx] + y2[idx] + y3[idx] + y4[idx] + y5[idx];
            y1[idx] += 2.0*y2[idx] + 3.0*y3[idx] + 4.0*y4[idx] + 5.0*y5[idx];
            y2[idx] += 3.0*y3[idx] + 6.0*y4[idx] + 10.0*y5[idx];
            y3[idx] += 4.0*y4[idx] + 10.0*y5[idx];
            y4[idx] += 5.0*y5[idx];

            r[idx] = y0[idx];
            v[idx] = y1[idx]*dt_inv;
        }

        *t = (*true_t += dt);
        *gen = local_gen;
    }

}