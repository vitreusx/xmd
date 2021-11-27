#pragma once
#include <xmd/types/vec3_array.h>

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
                auto dx = r12_n - nat_dist;

                auto dx2 = dx * dx;
                *V += dx2 * (H1 + H2 * dx2);
                auto dV_dx = dx * (2.0f * H1 + 4.0f * H2 * dx2);

                auto r12_u = r12 / r12_n;
                auto f = dV_dx * r12_u;
                F[i1] -= f;
                F[i2] += f;
            }
        }
    };
}
