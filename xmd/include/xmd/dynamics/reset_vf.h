#pragma once

#include <xmd/types/vec3.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class reset_vf {
    public:
        span<vec3r> F;
        real *V;



    public:
        void operator()() const;
        void omp_async() const;
    };
}