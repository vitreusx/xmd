#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class pc_integrator_step {
    public:
        vec3f_array r, v, F;
        // TODO: store them as doubles here for proper precision
        vec3f_array y0, y1, y2, y3, y4, y5;
        array<float> mass_inv;
        float *t, dt;
        int num_particles;

    public:
        inline void operator()() {
            auto dt_inv = 1.0f/dt;
            for (int idx = 0; idx < num_particles; ++idx) {
                auto error = y2[idx] - F[idx] * mass_inv[idx] * (dt*dt/2.0f);

                y0[idx] -= 3.0f/16.0f * error;
                y1[idx] -= 251.0f/360.0f * error;
                y2[idx] -= 1.0f * error;
                y3[idx] -= 11.0f/18.0f * error;
                y4[idx] -= 1.0f/6.0f * error;
                y5[idx] -= 1.0/60.0 * err;

                y0[idx] += y1[idx] + y2[idx] + y3[idx] + y4[idx] + y5[idx];
                y1[idx] += 2.0f*y2[idx] + 3.0f*y3[idx] + 4.0f*y4[idx] + 5.0f*y5[idx];
                y2[idx] += 3.0f*y3[idx] + 6.0f*y4[idx] + 10.0f*y5[idx];
                y3[idx] += 4.0f*y4[idx] + 10.0f*y5[idx];
                y4[idx] += 5.0f*y5[idx];

                r[idx] = y0[idx];
                v[idx] = y1[idx]*dt_inv;
            }

            *t += dt;
        }
    };
}