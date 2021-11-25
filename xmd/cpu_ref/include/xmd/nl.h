#pragma once
#include <xmd/meta/vector.h>
#include <xmd/types/vec3.h>
#include "box.h"

namespace xmd {
    class verify_integrity {
    public:
        xmd::list<vec3f> r, orig_r;
        boxf *box, *orig_box;
        float *pad;

    public:
        void operator()() const;
    };

    class
}

#include "detail/nl.inl"
