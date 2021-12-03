#include "forces/tether.h"

namespace xmd {
    void eval_tether_forces::operator()() const {
        for (int idx = 0; idx < tethers.size; ++idx) {
            auto i1 = tethers.i1[idx], i2 = tethers.i2[idx];
            auto nat_dist = tethers.nat_dist[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = r2 - r1;

            auto r12_n = norm(r12);
            auto [V_, dV_dr] = harmonic(H1, H2, nat_dist)(r12_n);

            auto r12_u = r12 / r12_n;
            *V += V_;
            F[i1] -= dV_dr * r12_u;
            F[i2] += dV_dr * r12_u;
        }
    }
}