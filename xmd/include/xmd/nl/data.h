#pragma once
#include <xmd/types/scalar.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "pair.h"

namespace xmd::nl {
    class nl_data {
    public:
        vector<nl_pair> particle_pairs;
        real orig_pad, ref_t;
        box orig_box;
        vector<vec3r> orig_r;
    };
}