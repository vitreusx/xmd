#pragma once

namespace xmd {
    void compute_relative_es::operator()() {
        auto V_factor = 1.0f / (4.0f * (float)M_PI * A);

        for (size_t i = 0; i < pairs.size(); ++i) {
            const_ref<es_pair> p = pairs[i];
            auto r1 = r[p.i1], r2 = r[p.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
            auto r12_u = r12 * r12_rn;

            auto Vij = V_factor * p.q1_q2 * expf(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
            auto dVij_dr = -Vij*(screen_dist_inv+2.0f*r12_rn);

            *V += Vij;
            auto f = r12_u * dVij_dr;
            F[p.i1] -= f;
            F[p.i2] += f;
        }
    }
}