#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class reset_vf {
    public:
        vec3r_array F;
        real *V;
        int num_particles;

    public:
        void operator()() const;
    };
}