#pragma once
#include <xmd/types/vec3.h>
#include "native.h"

namespace xmd {
    class eval_snd_forces {
    public:
        real CDH;
        const_array<vec3r> r;
        array<vec3r> F;
        const_span<nat_dih> dihedrals;
        real *V;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
