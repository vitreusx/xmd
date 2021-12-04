#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_total_energy {
    public:
        vec3r_array v;
        array<real> mass;
        int num_particles;
        real *V, *E;

    public:
        void operator()() const;
    };
}