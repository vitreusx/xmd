#pragma once
#include "candidate.h"
#include "sync_data.h"
#include "contact.h"
#include "free_pair.h"

namespace xmd::qa {
    class process_candidates {
    public:
        candidate_list *candidates;
        sync_data_array sync;
        real *t;
        contact_set *contacts;
        free_pair_set *free_pairs;

    public:
        void operator()() const;
    };
}