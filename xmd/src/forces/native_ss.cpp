#include "forces/native_ss.h"

namespace xmd {

    void update_nat_ssbonds::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {

            int pair_idx = pairs->push_back();
            pairs->i1[pair_idx] = idx1;
            pairs->i2[pair_idx] = idx2;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = cutoff;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < bonds.size; ++idx) {
            auto cys_i1 = bonds.i1[idx], cys_i2 = bonds.i2[idx];
            auto r1 = r[cys_i1], r2 = r[cys_i2];
            auto r12 = box->ray(r1, r2);

            auto r12_n = norm(r12);
            auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

            auto r12_u = r12 / r12_n;
            *V += V_;
            F[cys_i1] += dV_dr * r12_u;
            F[cys_i2] -= dV_dr * r12_u;
        }
    }
}