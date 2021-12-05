#include "forces/velocity_afm.h"

namespace xmd {

    void eval_velocity_afm_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto pulled_idx = bundles.pulled_idx[idx];
            auto r_ = r[pulled_idx];

            auto afm_orig = bundles.afm_orig[idx], afm_vel = bundles.afm_vel[idx];
            auto cur_afm_pos = afm_orig + *t * afm_vel;

            auto r_afm = cur_afm_pos - r_;
            auto r_afm_n = norm(r_afm), r_afm_rn = 1.0f / r_afm_n;
            auto r_afm_u = r_afm * r_afm_rn;

            auto [_, dV_dr] = afm_force(r_afm_n);
            F[pulled_idx] += dV_dr * r_afm_u;
        }
    }
}