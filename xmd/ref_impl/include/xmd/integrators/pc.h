#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class pc_integrator_step {
    public:
        vec3f_array r, v, F;
        vec3f_array y0, y1, y2, y3, y4, y5;
        array<float> mass_inv;
        float *t, dt;
        int num_particles;

    public:
        void operator()() const;
    };
}