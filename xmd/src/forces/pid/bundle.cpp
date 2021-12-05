#include "forces/pid/bundle.h"

namespace xmd::pid {

    void update_pid_bundles::operator()() const {
        bundles->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            auto prev1 = prev[idx1], next1 = next[idx1];
            auto prev2 = prev[idx2], next2 = next[idx2];
            auto atype1 = atype[idx1], atype2 = atype[idx2];

            int bundle_idx = bundles->push_back();
            bundles->i1p[bundle_idx] = prev1;
            bundles->i1[bundle_idx] = idx1;
            bundles->i1n[bundle_idx] = next1;
            bundles->i2p[bundle_idx] = prev2;
            bundles->i2[bundle_idx] = idx2;
            bundles->i2n[bundle_idx] = next2;
            int16_t type = (int16_t)atype1 +
                           (int16_t)amino_acid::NUM_AA * (int16_t)atype2;
            bundles->type[bundle_idx] = type;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = cutoff;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }
}