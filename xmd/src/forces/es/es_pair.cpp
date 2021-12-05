#include "forces/es/es_pair.h"

namespace xmd {

    void update_es_pairs::operator()() const {
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
}