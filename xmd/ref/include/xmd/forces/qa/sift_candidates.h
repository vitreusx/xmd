#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact_type.h"
#include "free_pair.h"
#include "sync_data.h"
#include "candidate.h"

namespace xmd::qa {
    class sift_candidates {
    public:
        float min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        float req_min_dist[contact_type::NUM_TYPES];
        polarization_type ptype[amino_acid::NUM_AA];

    public:
        vec3f_array r, n, h;
        box<vec3f> *box;
        array<amino_acid> atype;
        sync_data_array sync;

        free_pair_set *free_pairs;
        candidate_list *candidates;

    public:
        void operator()() const;
    };
}