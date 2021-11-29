#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/units.h>

namespace xmd {
    template<typename Random>
    class add_langevin_dynamics {
    public:
        Random rand;
        float gamma_factor, temperature;

    public:
        vec3f_array F, v;
        array<float> mass;
        int num_particles;

    public:
        void operator()() {
            auto noise_factor = sqrt(2.0f * kB * temperature);

            for (int idx = 0; idx < num_particles; ++idx) {
                auto gamma = gamma_factor * mass[idx];
                auto noise_std = noise_factor * sqrt(gamma);
                auto noise_x = rand.normal(0.0f, noise_std);
                auto noise_y = rand.normal(0.0f, noise_std);
                auto noise_z = rand.normal(0.0f, noise_std);
                auto noise = vec3f(noise_x, noise_y, noise_z);
                F[idx] += -gamma * v[idx] + noise;
            }
        }
    };
}