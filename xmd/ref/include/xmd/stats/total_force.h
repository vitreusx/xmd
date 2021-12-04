#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_total_force {
    public:
        vec3r_span F;
        vec3r *total_F;

    public:
        void operator()();
    };
}