#pragma once

#include <xmd/types/vec3.h>

namespace xmd {
    class compute_rmsd {
    public:
        real *rmsd;
        const_array<vec3r> r;
        array<vec3r> ref_r;
        int num_particles;



    public:
        void operator()() const;
    };
}