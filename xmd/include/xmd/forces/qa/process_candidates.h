#pragma once
#include "candidate.h"
#include "sync_data.h"
#include "contact.h"
#include "free_pair.h"
#include <xmd/vm/vm.h>

namespace xmd::qa {
    class process_candidates: public vm_aware {
    public:
        candidate_list *candidates;
        sync_data_array sync;
        real *t;
        contact_set *contacts;
        free_pair_set *free_pairs;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}