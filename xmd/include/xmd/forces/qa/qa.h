#pragma once
#include "precompute_nh.h"
#include "sift_candidates.h"
#include "process_candidates.h"
#include "process_contacts.h"


namespace xmd::qa {
    class eval_qa_forces {
    public:
        precompute_nh precompute_nh_t;
        sift_candidates sift_candidates_t;
        process_candidates process_candidates_t;
        process_contacts process_contacts_t;



    public:
        void operator()() const;
    };

    class update_qa {
    public:
        update_free_pairs update_free_pairs_;



    public:
        void operator()() const;
    };
}