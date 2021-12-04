#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>

namespace xmd {
    struct tether_pair_span {
        array<int> i1, i2;
        array<real> nat_dist;
        int size;
    };

    class eval_tether_forces {
    public:
        real H1, H2;

    public:
        vec3r_array r, F;
        tether_pair_span tethers;
        real *V;

    public:
        void operator()() const;
    };
}
