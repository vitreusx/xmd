#pragma once
#include <xmd/forces/qa/contact_type.h>
#include <xmd/forces/qa/free_pair.h>
#include <xmd/types/vec3_array.h>

namespace xmd {
    struct candidate_array {
        int *i1, *i2;
    };



    class sift_candidates {
    public:
        float min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        float req_min_dist;

    public:

    };
}