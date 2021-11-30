#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class reset_vf {
    public:
        vec3f_array F;
        float *V;
        int num_particles;

    public:
        inline void operator()() {
            *V = 0.0f;
            for (int idx = 0; idx < num_particles; ++idx) {
                F[idx] = vec3f::Zero();
            }
        }
    };
}