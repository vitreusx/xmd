#pragma once
#include <xmd/types/vec3.h>


namespace xmd {
    class compute_total_energy {
    public:
        const_array<vec3r> v;
        array<real> mass;
        int num_particles;
        real *K, *V, *E;



    public:
        void operator()() const;
    };
}