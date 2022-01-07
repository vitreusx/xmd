#pragma once
#include "candidate.h"
#include "sync_data.h"
#include "contact.h"
#include "free_pair.h"
#include <xmd/ctx/context.h>

namespace xmd::qa {
    class process_candidates: public ctx_aware {
    public:
        set<candidate> *candidates;
        array<sync_data> sync;
        real const *t;
        set<contact> *contacts;
        set<free_pair> *free_pairs;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()() const;
    };
}