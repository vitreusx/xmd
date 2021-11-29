#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class leapfrog_integrator_step {
    public:
        vec3f_array r, v, F;
        vec3f_array a_prev;
        array<float> mass_inv;
        float *t, dt;
        int num_particles;

    public:
        inline void operator()() {
            for (int idx = 0; idx < num_particles; ++idx) {
                auto a_cur = F[idx] * mass_inv[idx];
                v[idx] += 0.5f * (a_prev[idx] + a_cur) * dt;
                r[idx] += v[idx] * dt + 0.5f * a_cur * dt * dt;
                a_prev[idx] = a_cur;
            }
            *t += dt;
        }
    };
}