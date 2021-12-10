#pragma once
#include "precompute_nh.h"
#include "sift_candidates.h"
#include "process_candidates.h"
#include "process_contacts.h"
#include <xmd/vm/vm.h>

namespace xmd::qa {
    class eval_qa_forces: public vm_aware {
    public:
        precompute_nh precompute_nh_t;
        sift_candidates sift_candidates_t;
        process_candidates process_candidates_t;
        process_contacts process_contacts_t;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}