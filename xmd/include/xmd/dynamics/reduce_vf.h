#pragma once
#include <xmd/types/vec3.h>


namespace xmd {
    class reduce_vf {
    public:
        array<vec3r> F, thread_F;
        real *V, *thread_V;
        int num_particles;



    public:
        void operator()() const;
    };
}