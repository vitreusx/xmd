#pragma once
#include <xmd/types/vec3_array.h>
#include <xmd/forces/primitives/harmonic.h>

namespace xmd {
    struct tether_pair_array {
        int *i1, *i2;
        float *nat_dist;
        int size;
    };

    class eval_tether_forces {
    public:
        float H1, H2;

    public:
        vec3f_array r, F;
        tether_pair_array tethers;
        float *V;

    public:
        inline void operator()() const {
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
    };
}
