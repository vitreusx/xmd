#pragma once
#include "univ/vector3.h"

namespace fastmd {
    class qa {
    public:
        vector3f *r, *F;
        float *V;


    };
}

#include "detail/qa.inl"
