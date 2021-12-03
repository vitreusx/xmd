#include "forces/dynss/process_contacts.h"

namespace xmd::dynss {

    void process_contacts::operator()() const {
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            if (!contacts->has_item(idx))
                continue;

            auto i1 = contacts->i1[idx], i2 = contacts->i2[idx];
            auto status = contacts->status[idx];
            auto ref_time = contacts->ref_time[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
            auto r12_u = r12 * r12_rn;

            auto saturation = fminf(*t - ref_time, cycle_time) * cycle_time_inv;
            if (status == BREAKING)
                saturation = 1.0f - saturation;

            auto [V_, dV_dr] = disulfide_force(r12_n);
            *V += saturation * V_;
            F[i1] += saturation * dV_dr * r12_u;
            F[i2] -= saturation * dV_dr * r12_u;

            if (status == FORMING_OR_FORMED && saturation == 1.0f) {
                auto cys1_num_neigh = num_neighbours[i1];
                auto cys2_num_neigh = num_neighbours[i2];

                auto beyond_range = (abs(r12_n - optimal_dist) > max_dist_deviation);
                auto not_enough_neigh = (cys1_num_neigh + cys2_num_neigh < min_req_num_of_neighbours);
                if (beyond_range && not_enough_neigh) {
                    contacts->status[idx] = BREAKING;
                    contacts->ref_time[idx] = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
                contacts->remove(idx);
                sync[i1] += ssbond_sync_diff;
                sync[i2] += ssbond_sync_diff;
            }
        }
    }
}