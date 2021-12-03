#include "forces/langevin.h"

namespace xmd {
    void eval_langevin_dynamics::operator()() const {
        xorshift64 local_gen = *gen;

        auto noise_factor = sqrt(2.0f * kB * temperature);

        for (int idx = 0; idx < num_particles; ++idx) {
            auto gamma = gamma_factor * mass[idx];
            auto noise_std = noise_factor * sqrt(gamma);

            auto [noise_x, noise_y] = local_gen.normalx2();
            auto noise_z = local_gen.normalx2().first;
            auto noise = vec3f(noise_x, noise_y, noise_z);

            F[idx] += -gamma * v[idx] + noise_std * noise;
        }

        *gen = local_gen;
    }
}