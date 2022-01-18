#include "forces/es/relative.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_relative_es_forces::operator()() const {
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_relative_es_forces::iter(int idx) const {
        auto es = es_pairs->at(idx);
        auto i1 = es.i1(), i2 = es.i2();
        auto q1_x_q2 = es.q1_x_q2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
        auto r12_u = r12 * r12_rn;

        auto Vij = V_factor * q1_x_q2 * exp(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
        auto dVij_dr = -Vij*(screen_dist_inv+2.0f*r12_rn);

        *V += Vij;

        auto f = r12_u * dVij_dr;
        F[i1] += f;
        F[i2] -= f;

    }

    void eval_relative_es_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void update_relative_es::operator()() const {
        update_es_base::operator()();
    }
}