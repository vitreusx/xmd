#pragma once
#include "precompute_nh.h"
#include "sift_candidates.h"
#include "process_candidates.h"
#include "process_contacts.h"
#include <xmd/ctx/context.h>

namespace xmd::qa {
    class eval_qa_forces: public ctx_aware {
    public:
        precompute_nh precompute_nh_t;
        sift_candidates sift_candidates_t;
        process_candidates process_candidates_t;
        process_contacts process_contacts_t;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };

    class update_qa: public ctx_aware {
    public:
        update_free_pairs update_free_pairs_;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}