#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"

namespace xmd {
    class eval_const_es_forces {
    public:
        real permittivity, screen_dist_inv;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        es_pair_span es_pairs;
        real *V;

    public:
        void operator()() const;
    };
}
