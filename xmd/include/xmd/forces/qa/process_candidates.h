#pragma once
#include "candidate.h"
#include "sync_data.h"
#include "contact.h"
#include "free_pair.h"


namespace xmd::qa {
    class process_candidates {
    public:
        set<candidate> *candidates;
        array<sync_data> sync;
        real const *t;
        set<contact> *contacts;
        set<free_pair> *free_pairs;



    public:
        void iter(int idx) const;
        void operator()() const;
    };
}