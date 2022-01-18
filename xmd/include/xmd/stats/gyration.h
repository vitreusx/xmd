#pragma once

#include <xmd/types/vec3.h>

namespace xmd {
    class compute_gyration_radius {
    public:
        real* gyration_radius;
        const_array<vec3r> r;
        array<real> mass;
        int num_particles;



    public:
        void operator()() const;
    };
}