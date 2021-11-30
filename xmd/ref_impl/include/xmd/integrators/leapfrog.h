#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>

namespace xmd {
    class perform_leapfrog_step {
    public:
        vec3f_array r, v, F;
        vec3f_array a_prev;
        array<float> mass_inv;
        float *t, dt;
        int num_particles;

    public:
        void operator()() const;
    };
}