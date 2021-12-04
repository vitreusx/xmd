#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_total_force {
    public:
        vec3f_span F;
        vec3f *total_F;

    public:
        void operator()();
    };
}