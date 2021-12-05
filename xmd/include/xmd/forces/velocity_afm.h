#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>

namespace xmd {
    struct velocity_afm_bundle_span {
        array<int> pulled_idx;
        vec3r_array afm_orig, afm_vel;
        int size;
    };

    class eval_velocity_afm_forces {
    public:
        harmonic afm_force;

    public:
        vec3r_array r, F;
        real *t;
        velocity_afm_bundle_span bundles;

    public:
        void operator()() const;
    };
}