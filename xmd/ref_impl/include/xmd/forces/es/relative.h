#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"

namespace xmd {
    class eval_relative_es_forces {
    public:
        float A, screen_dist_inv;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        es_pair_span es_pairs;
        float *V;

    public:
        void operator()() const;
    };
}
