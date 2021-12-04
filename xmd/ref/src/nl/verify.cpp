#include "nl/verify.h"

namespace xmd::nl {

    void verify::operator()() const {
        real max_r_disp = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            auto r_ = r[idx], orig_r_ = orig_r[idx];
            auto dr = box->ray(r_, orig_r_);
            max_r_disp = max(max_r_disp, norm(dr));
        }

        auto box_disp = l1_norm(box->cell - orig_box->cell);

        real total_disp = 2.0f * max_r_disp + box_disp;
        if (total_disp > pad)
            *invalid = true;
    }
}