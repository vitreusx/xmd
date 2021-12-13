#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact_type.h"
#include "free_pair.h"
#include "sync_data.h"
#include "candidate.h"
#include <xmd/vm/vm.h>
#include <mutex>
#include <taskflow/taskflow.hpp>

namespace xmd::qa {
    class sift_candidates: public vm_aware {
    public:
        real min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        real req_min_dist[contact_type::NUM_TYPES];
        polarization_type ptype[amino_acid::NUM_AA];

    public:
        vec3r_array r, n, h;
        box<vec3r> *box;
        array<amino_acid> atype;
        sync_data_array sync;
        std::mutex *mut;

        free_pair_set *free_pairs;
        candidate_list *candidates;

        void init_from_vm(vm& vm_inst) override;

    public:
        void loop_iter(int idx) const;
        void operator()() const;
    };

    class sift_candidates_tf: public vm_aware {
    public:
        sift_candidates sift_candidates_;
        tf::Task clear, loop;
        tf::Taskflow module;
        void init_from_vm(vm& vm_inst) override;

    public:
        tf::Task tf_impl(tf::Taskflow& taskflow);
    };
}