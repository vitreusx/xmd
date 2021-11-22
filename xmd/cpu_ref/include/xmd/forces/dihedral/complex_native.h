#pragma once
#include "detail/native_dihedral.h"

namespace xmd {
    class compute_complex_native_dihedrals {
    public:
        float cda, cdb;

        xmd::list<vec3f> r;
        xmd::list<native_dihedral> dihedrals;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/simple_native.inl"
