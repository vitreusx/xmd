#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact_type.h"
#include "free_pair.h"
#include "sync_data.h"
#include "candidate.h"
#include <xmd/vm/vm.h>

namespace xmd::qa {
    class sift_candidates: public vm_aware {
    public:
        real min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        real req_min_dist[contact_type::NUM_TYPES];
        polarization_type ptype[amino_acid::NUM_AA];

    public:
        const_array<vec3r> r, n, h;
        box<vec3r> const *box;
        array<amino_acid> atype;
        const_array<sync_data> sync;

        set<free_pair> const *free_pairs;
        set<candidate> *candidates;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;

        void omp_prep() const;
        void omp_async() const;
    };
}