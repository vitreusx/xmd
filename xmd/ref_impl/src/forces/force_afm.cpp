#include "forces/force_afm.h"

namespace xmd {

    void eval_force_afm_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto pulled_idx = bundles.pulled_idx[idx];
            F[pulled_idx] += bundles.pull_force[idx];
        }
    }
}