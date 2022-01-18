#include "forces/force_afm.h"
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    void eval_force_afm_forces::operator()() const {
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_force_afm_forces::iter(int idx) const {
        auto tip = afm_tips[idx];
        F[tip.res_idx()] += tip.pull_force();
    }

    void eval_force_afm_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }
}