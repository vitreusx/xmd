#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    struct force_afm_bundle_span {
        array<int> pulled_idx;
        vec3r_array pull_force;
        int size;
    };

    class eval_force_afm_forces {
    public:
        vec3r_array F;
        force_afm_bundle_span bundles;

    public:
        void operator()() const;
    };
}