#include "forces/qa/process_contacts.h"

namespace xmd::qa {

    void process_contacts::operator()() const {
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            if (!contacts->has_item(idx))
                continue;

            auto i1 = contacts->i1[idx], i2 = contacts->i2[idx];
            auto type = contacts->type[idx];
            auto status = contacts->status[idx];
            auto ref_time = contacts->ref_time[idx];
            auto sync_diff1 = contacts->sync_diff1[idx];
            auto sync_diff2 = contacts->sync_diff2[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);
            auto r12_u = r12 * r12_rn;

            auto saturation = fminf(*t - ref_time, cycle_time) * cycle_time_inv;
            if (status == BREAKING)
                saturation = 1.0f - saturation;

            auto lj_force = ljs[(short)type];
            auto [Vij, dVij_dr] = lj_force(r12_rn);
            *V += saturation * Vij;
            auto f = saturation * dVij_dr * r12_u;
            F[i1] -= f;
            F[i2] += f;

            if (status == FORMING_OR_FORMED && saturation == 1.0f) {
                if (breaking_factor * powf(2.0f, -1.0f/6.0f) * lj_force.r_min * r12_rn < 1.0f) {
                    contacts->status[idx] = BREAKING;
                    contacts->ref_time[idx] = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
                contacts->remove(idx);
                sync[i1] += sync_diff1;
                sync[i2] += sync_diff2;
            }
        }
    }
}