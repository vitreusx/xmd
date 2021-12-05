#include "forces/pauli.h"

namespace xmd {

    void update_pauli_pairs::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            int pair_idx = pairs->push_back();
            pairs->i1[pair_idx] = idx1;
            pairs->i2[pair_idx] = idx2;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = r_excl;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }

    void eval_pauli_exclusion_forces::operator()() const {
        for (int idx = 0; idx < pairs.size; ++idx) {
            auto i1 = pairs.i1[idx], i2 = pairs.i2[idx];
            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);

            auto within = (1.0f < r12_rn * r_excl);
            if (within) {
                auto r12_u = r12 * r12_rn;
                auto [V_, dV_dr] = lj(depth, r_excl)(r12_rn);

                *V += V_;
                F[i1] += r12_u * dV_dr;
                F[i2] -= r12_u * dV_dr;
            }
        }
    }
}