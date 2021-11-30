#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>

namespace xmd {
    class perform_leapfrog_step {
    public:
        double dt;

    public:
        vec3f_array r, v, F;
        vec3d_array true_r, true_v, a_prev;
        array<float> mass_inv;
        double *true_t;
        float *t;
        int num_particles;

    public:
        void operator()() const;
    };
}