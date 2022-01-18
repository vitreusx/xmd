#include "forces/qa/process_candidates.h"

namespace xmd::qa {
    void process_candidates::operator()() const {
        for (int idx = 0; idx < candidates->extent(); ++idx) {
            iter(idx);
        }
    }

    void process_candidates::iter(int idx) const {
        auto candidate = candidates->at(idx).value();
        auto i1 = candidate.i1(), i2 = candidate.i2();
        auto sync_diff1 = candidate.sync_diff1();
        auto sync_diff2 = candidate.sync_diff2();
        auto type = candidate.type();

        sync_data new_sync1 = sync[i1] - sync_diff1;
        sync_data new_sync2 = sync[i2] - sync_diff2;

        if (new_sync1.is_valid() && new_sync2.is_valid()) {
            contacts->emplace(i1, i2, type, FORMING_OR_FORMED, *t,
                sync_diff1, sync_diff2);

            sync[i1] -= sync_diff1;
            sync[i2] -= sync_diff2;

            free_pairs->remove(candidate.free_pair_idx());
        }
    }
}