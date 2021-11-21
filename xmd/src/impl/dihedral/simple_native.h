#pragma once
#include "detail/native_dihedral.h"

namespace xmd {
    class compute_simple_native_dihedrals {
    public:
        float cdh;

        xmd::list<vec3f> r;
        xmd::list<native_dihedral> dihedrals;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/simple_native.inl"
