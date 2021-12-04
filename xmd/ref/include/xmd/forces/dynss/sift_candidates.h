#pragma once
#include <xmd/forces/qa/contact_type.h>
#include <xmd/forces/qa/free_pair.h>
#include <xmd/forces/qa/sync_data.h>
#include <xmd/types/vec3.h>
#include "candidate.h"

namespace xmd::dynss {
    class sift_candidates {
    public:
        real max_cos_nr;
        real req_min_dist;
        qa::sync_data def_sync_diff;

    public:
        vec3r_array r, n;
        array<bool> has_formed_ssbond;
        box<vec3r> *box;
        qa::sync_data_array sync;

        qa::free_pair_set *free_cys_pairs;
        candidate_list *candidates;

    public:
        void operator()() const;
    };
}