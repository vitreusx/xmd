#pragma once

namespace xmd {
    void compute_pauli::operator()() {
        for (size_t i = 0; i < pairs.size(); ++i) {
            const_ref<pauli_pair> p = pairs[i];
            auto r1 = r[p.i1], r2 = r[p.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_rn = rnorm(r12);
            auto r12_u = r12 * r12_rn;

            auto within = (1.0f < r12_rn * r_excl);
            if (within) {
                auto s = r12_rn * r_excl;
                auto s6 = s * s * s * s * s * s, s12 = s6 * s6;
                *V = depth * (s12 - 2.0f * s6 + 1.0f);

                auto dV_dr = 12.0f * depth * r12_rn * (s6 - s12);
                F[p.i1] -= r12_u * dV_dr;
                F[p.i2] += r12_u * dV_dr;
            }
        }
    }
}