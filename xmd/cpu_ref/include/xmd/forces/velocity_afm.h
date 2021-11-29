#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>

namespace xmd {
    struct velocity_afm_bundle_array {
        int *pulled_idx;
        vec3f_array afm_orig, afm_vel;
        int size;
    };

    class eval_velocity_afm_forces {
    public:
        harmonic afm_force;

    public:
        vec3f_array r, F;
        float *t;
        velocity_afm_bundle_array bundles;

    public:
        void operator()() {
            for (int idx = 0; idx < bundles.size; ++idx) {
                auto pulled_idx = bundles.pulled_idx[idx];
                auto r_ = r[pulled_idx];

                auto afm_orig = bundles.afm_orig[idx], afm_vel = bundles.afm_vel[idx];
                auto cur_afm_pos = afm_orig + *t * afm_vel;

                auto r_afm = cur_afm_pos - r_;
                auto r_afm_n = norm(r_afm), r_afm_rn = 1.0f / r_afm_n;
                auto r_afm_u = r_afm * r_afm_rn;

                auto [_, dV_dr] = afm_force(r_afm_n);
                F[pulled_idx] -= dV_dr * r_afm_u;
            }
        }
    };
}