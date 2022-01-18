#include "forces/tether.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    void eval_tether_forces::operator()() const {
        for (int idx = 0; idx < tethers.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_tether_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < tethers.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_tether_forces::iter(int idx) const {
        auto tether = tethers[idx];
        auto i1 = tether.i1(), i2 = tether.i2();
        auto nat_dist = tether.nat_dist();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = r2 - r1;

        auto r12_n = norm(r12);
        auto [V_, dV_dr] = harmonic(H1, H2, nat_dist)(r12_n);

        auto r12_u = r12 / r12_n;
        *V += V_;
        F[i1] += dV_dr * r12_u;
        F[i2] -= dV_dr * r12_u;
    }
}