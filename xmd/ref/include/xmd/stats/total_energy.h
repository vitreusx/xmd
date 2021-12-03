#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_total_energy {
    public:
        vec3f_array v;
        array<float> mass;
        int num_particles;
        float *V, *E;

    public:
        void operator()() const;
    };
}