#pragma once
#include "candidate.h"
#include <xmd/forces/qa/sync_data.h>
#include "contact.h"

namespace xmd::dynss {
    class process_candidates {
    public:
        qa::sync_data ssbond_sync_diff;

    public:
        candidate_list *candidates;
        qa::sync_data_array sync;
        float *t;
        contact_set *contacts;

    public:
        void operator()() const;
    };
}