#include "forces/qa/process_candidates.h"

namespace xmd::qa {

    void process_candidates::operator()() const {
        for (int idx = 0; idx < candidates->size(); ++idx) {
            auto i1 = candidates->i1[idx], i2 = candidates->i2[idx];
            auto sync_diff1 = candidates->sync_diff1[idx];
            auto sync_diff2 = candidates->sync_diff2[idx];
            auto type = candidates->type[idx];

            auto new_sync1 = sync[i1] - sync_diff1;
            auto new_sync2 = sync[i2] - sync_diff2;

            if (new_sync1.is_valid() && new_sync2.is_valid()) {
                int slot_idx = contacts->add();
                contacts->i1[slot_idx] = i1;
                contacts->i2[slot_idx] = i2;
                contacts->type[slot_idx] = type;
                contacts->sync_diff1[slot_idx] = sync_diff1;
                contacts->sync_diff2[slot_idx] = sync_diff2;
                contacts->ref_time[slot_idx] = *t;
                contacts->status[slot_idx] = FORMING_OR_FORMED;

                sync[i1] -= sync_diff1;
                sync[i2] -= sync_diff2;

                
            }
        }
    }
}