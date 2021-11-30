#include "forces/dynss/process_candidates.h"

namespace xmd::dynss {

    void process_candidates::operator()() const {
        for (int idx = 0; idx < candidates->size(); ++idx) {
            auto i1 = candidates->i1[idx], i2 = candidates->i2[idx];

            auto new_sync1 = sync[i1] - ssbond_sync_diff;
            auto new_sync2 = sync[i2] - ssbond_sync_diff;

            if (new_sync1.is_valid() && new_sync2.is_valid()) {
                int slot_idx = contacts->add();
                contacts->i1[slot_idx] = i1;
                contacts->i2[slot_idx] = i2;
                contacts->status[slot_idx] = FORMING_OR_FORMED;

                sync[i1] -= ssbond_sync_diff;
                sync[i2] -= ssbond_sync_diff;
            }
        }
    }
};
