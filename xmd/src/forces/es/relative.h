#pragma once
#include "detail/es_pair.h"

namespace xmd {
    class compute_relative_es {
    public:
        float A, screen_dist_inv;

        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<es_pair> pairs;

        xmd::list<vec3f> F;
        float *V;

    public:
        void operator()();
    };
}

#include "detail/relative.inl"
