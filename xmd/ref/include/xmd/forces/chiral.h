#pragma once
#include <xmd/types/vec3.h>
#include <xmd/math.h>

namespace xmd {
    struct chiral_quad_span {
        array<int> i1, i2, i3, i4;
        array<real> nat_chir, nat_factor;
        int size;
    };

    class eval_chiral_forces {
    public:
        real e_chi;

    public:
        vec3r_array r, F;
        chiral_quad_span quads;
        real *V;

    public:
        void fill_nat_values() const;

    public:
        void operator()() const;
    };
}
