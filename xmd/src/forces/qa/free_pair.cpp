#include "forces/qa/free_pair.h"

namespace xmd::qa {

    void update_free_pairs::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            int pair_idx = pairs->add();
            pairs->i1[pair_idx] = idx1;
            pairs->i2[pair_idx] = idx2;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = max_formation_min_dist;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }
}