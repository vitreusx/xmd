#include "dynamics/pc.h"

namespace xmd {
    void pc_integrator_step::operator()() const {
        auto dt_inv = 1.0/dt;
        for (int idx = 0; idx < num_particles; ++idx) {
            auto error = y2[idx] - F[idx] * mass_inv[idx] * (dt*dt/2.0);
            y0[idx] -= 3.0/16.0 * error;
            y1[idx] -= 251.0/360.0 * error;
            y2[idx] -= 1.0 * error;
            y3[idx] -= 11.0/18.0 * error;
            y4[idx] -= 1.0/6.0 * error;
            y5[idx] -= 1.0/60.0 * error;

            y0[idx] += y1[idx] + y2[idx] + y3[idx] + y4[idx] + y5[idx];
            y1[idx] += 2.0*y2[idx] + 3.0*y3[idx] + 4.0*y4[idx] + 5.0f*y5[idx];
            y2[idx] += 3.0*y3[idx] + 6.0*y4[idx] + 10.0*y5[idx];
            y3[idx] += 4.0*y4[idx] + 10.0*y5[idx];
            y4[idx] += 5.0*y5[idx];

            r[idx] = y0[idx];
            v[idx] = y1[idx]*dt_inv;
        }

        *true_t += dt;
        *t = (float)*true_t;
    }
}