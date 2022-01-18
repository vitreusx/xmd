#pragma once

#include <xmd/types/vec3.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class reset_thread_vf {
    public:
        span<vec3r> thread_F;
        real *thread_V;



    public:
        void operator()() const;
    };
}