#pragma once
#include "candidate.h"
#include "sync_data.h"
#include "contact.h"

namespace xmd::qa {
    class process_candidates {
    public:
        candidate_list *candidates;
        sync_data_array sync;
        float *t;
        contact_set *contacts;

    public:
        void operator()() const;
    };
}