#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/units.h>

namespace xmd {
    template<typename Generator>
    class eval_langevin_dynamics {
    public:
        float gamma_factor, temperature;
        std::normal_distribution<float> noise_dist;

    public:
        Generator &g;
        vec3f_array F, v;
        array<float> mass;
        int num_particles;

    public:
        explicit eval_langevin_dynamics(Generator& g):
            g{g}, noise_dist(0.0f, 1.0f) {};

        void operator()() {
            auto noise_factor = sqrt(2.0f * kB * temperature);

            for (int idx = 0; idx < num_particles; ++idx) {
                auto gamma = gamma_factor * mass[idx];
                auto noise_std = noise_factor * sqrt(gamma);
                auto noise_x = noise_std * noise_dist(g);
                auto noise_y = noise_std * noise_dist(g);
                auto noise_z = noise_std * noise_dist(g);
                auto noise = vec3f(noise_x, noise_y, noise_z);
                F[idx] += -gamma * v[idx] + noise;
            }
        }
    };
}