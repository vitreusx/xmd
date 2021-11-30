#pragma once
#include "candidate.h"
#include <xmd/forces/qa/sync_data.h>
#include "contact.h"

namespace xmd::dynss {
    class process_candidates {
    public:
        qa::sync_data ssbond_sync_diff;

    public:
        candidate_list candidates;
        sync_data_array sync;
        float *t;
        contact_set contacts;

    public:
        inline void operator()() {
            for (int idx = 0; idx < candidates.extent(); ++idx) {
                auto i1 = candidates.i1[idx], i2 = candidates.i2[idx];

                auto new_sync1 = sync[i1] - ssbond_sync_diff;
                auto new_sync2 = sync[i2] - ssbond_sync_diff;

                if (new_sync1.is_valid() && new_sync2.is_valid()) {
                    int slot_idx = contacts.add();
                    contacts.i1[slot_idx] = i1;
                    contacts.i2[slot_idx] = i2;
                    contacts.status[slot_idx] = FORMING_OR_FORMED;

                    sync[i1] -= ssbond_sync_diff;
                    sync[i2] -= ssbond_sync_diff;
                }
            }
        }
    };
}