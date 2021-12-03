#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/types/amino_acid.h>

namespace xmd {
    struct native_angle_span {
        array<int> i1, i2, i3;
        array<float> nat_theta;
        int size;
    };

    class eval_native_angle_forces {
    public:
        float k;

    public:
        float *V;
        vec3f_array r, F;
        native_angle_span angles;

    public:
        void operator()() const;
    };
}
