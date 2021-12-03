#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>

namespace xmd {
    struct velocity_afm_bundle_span {
        array<int> pulled_idx;
        vec3f_array afm_orig, afm_vel;
        int size;
    };

    class eval_velocity_afm_forces {
    public:
        harmonic afm_force;

    public:
        vec3f_array r, F;
        float *t;
        velocity_afm_bundle_span bundles;

    public:
        void operator()() const;
    };
}