#pragma once

namespace xmd {
    struct force_afm_bundle_array {
        int *pulled_idx;
        vec3f_array pull_force;
        int size;
    };

    class eval_force_afm_forces {
    public:
        vec3f_array r, F;
        force_afm_bundle_array bundles;

    public:
        void operator()() {
            for (int idx = 0; idx < bundles.size; ++idx) {
                auto pulled_idx = bundles.pulled_idx[idx];
                auto r_ = r[pulled_idx];
                F[pulled_idx] += bundles.pull_force[idx];
            }
        }
    };
}