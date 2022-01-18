#include "forces/native_ss.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    void update_nat_ssbonds::operator()() const {
        ssbonds->clear();

        for (int idx = 0; idx < all_ssbonds->size(); ++idx) {
            auto ssbond = all_ssbonds->at(idx);
            auto i1 = ssbond.i1(), i2 = ssbond.i2();
            auto r1 = r[i1], r2 = r[i2];
            if (norm(box->r_uv(r1, r2)) < cutoff + nl->orig_pad) {
                ssbonds->emplace_back(i1, i2);
            }
        }
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_nat_ssbond_forces::iter(int idx) const {
        auto ssbond = ssbonds->at(idx);
        auto cys_i1 = ssbond.i1(), cys_i2 = ssbond.i2();
        auto r1 = r[cys_i1], r2 = r[cys_i2];
        auto r12 = box->r_uv(r1, r2);

        auto r12_n = norm(r12);
        auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

        auto r12_u = r12 / r12_n;
        *V += V_;
        F[cys_i1] += dV_dr * r12_u;
        F[cys_i2] -= dV_dr * r12_u;
    }

    void eval_nat_ssbond_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }
}