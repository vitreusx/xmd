#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/types/amino_acid.h>

namespace xmd {
    struct native_angle_span {
        array<int> i1, i2, i3;
        array<real> nat_theta;
        int size;
    };

    class eval_native_angle_forces {
    public:
        real k;

    public:
        real *V;
        vec3r_array r, F;
        native_angle_span angles;

    public:
        void operator()() const;
    };
}
