#pragma once
#include <xmd/types/vec3_array.h>
#include <xmd/model/box.h>
#include "es_pair_array.h"

namespace xmd {
    class eval_const_es_forces {
    public:
        float permittivity, screen_dist_inv;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        es_pair_span es_pairs;
        float *V;

    public:
        void operator()() const {
            auto V_factor = 1.0f / (4.0f * (float)M_PI * permittivity);

            for (int idx = 0; idx < es_pairs.size; ++idx) {
                auto i1 = es_pairs.i1[idx], i2 = es_pairs.i2[idx];
                auto q1_q2 = es_pairs.q1_q2[idx];

                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
                auto r12_u = r12 * r12_rn;

                auto Vij = V_factor * q1_q2 * expf(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
                auto dVij_dr = -Vij*(screen_dist_inv+r12_rn);

                *V += Vij;
                auto f = r12_u * dVij_dr;
                F[i1] -= f;
                F[i2] += f;
            }
        }
    };
}
