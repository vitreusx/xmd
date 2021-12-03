#include "forces/es/relative.h"

namespace xmd {

    void eval_relative_es_forces::operator()() const {
        auto V_factor = 1.0f / (4.0f * (float)M_PI * A);

        for (int idx = 0; idx < es_pairs.size; ++idx) {
            auto i1 = es_pairs.i1[idx], i2 = es_pairs.i2[idx];
            auto q1_q2 = es_pairs.q1_q2[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
            auto r12_u = r12 * r12_rn;

            auto Vij = V_factor * q1_q2 * expf(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
            auto dVij_dr = -Vij*(screen_dist_inv+2.0f*r12_rn);

            *V += Vij;
            auto f = r12_u * dVij_dr;
            F[i1] += f;
            F[i2] -= f;
        }
    }
}