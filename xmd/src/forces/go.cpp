#include "forces/go.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {
    void update_go_contacts::operator()() const {
        contacts->clear();

        for (int idx = 0; idx < all_contacts->size(); ++idx) {
            auto nat_cont = all_contacts->at(idx);
            auto idx1 = nat_cont.i1(), idx2 = nat_cont.i2();
            auto nat_dist = nat_cont.nat_dist();

            auto cutoff = lj::cutoff(nat_dist);

            auto r1 = r[idx1], r2 = r[idx2];
            if (norm(box->r_uv(r1, r2)) < cutoff + nl->orig_pad) {
                contacts->push_back(nat_cont);
            }
        }
    }

    void eval_go_forces::operator()() const {
        for (int idx = 0; idx < contacts->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_go_forces::iter(int idx) const {
        auto nat_cont = contacts->at(idx);
        auto i1 = nat_cont.i1(), i2 = nat_cont.i2();
        auto nat_dist = nat_cont.nat_dist();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_rn = norm_inv(r12);

        auto r12_u = r12 * r12_rn;
        auto [V_, dV_dr] = lj(depth, nat_dist)(r12_rn);

        *V += V_;
        F[i1] += r12_u * dV_dr;
        F[i2] -= r12_u * dV_dr;
    }

    void eval_go_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < contacts->size(); ++idx) {
            iter(idx);
        }
    }
}