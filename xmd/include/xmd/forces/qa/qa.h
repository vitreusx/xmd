#pragma once
#include "precompute_nh.h"
#include "sift_candidates.h"
#include "process_candidates.h"
#include "process_contacts.h"
#include <xmd/vm/vm.h>
#include <taskflow/taskflow.hpp>

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

    class eval_qa_forces_tf: public vm_aware {
    public:
        precompute_nh precompute_nh_t;
        sift_candidates_tf sift_candidates_tf_t;
        process_candidates process_candidates_t;
        process_contacts process_contacts_t;

        tf::Task precomp_nh_t, sift_cand_t, process_cand_t, process_cont_t;
        tf::Taskflow module;

        void init_from_vm(vm& vm_inst) override;

    public:
        tf::Task tf_impl(tf::Taskflow& taskflow);
    };
}