#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::nl {
    class verify {
    public:
        float cell_pad;

    public:
        vec3f_array r, orig_r;
        box<vec3f> *box, orig_box;
        bool *invalid;
        int num_particles;

    public:
        void operator()() {
            auto max_r_disp = 0.0f;
            for (int idx = 0; idx < num_particles; ++idx) {
                auto r_ = r[idx], orig_r_ = orig_r[idx];
                auto dr = box->ray(r_, orig_r_);
                max_r_disp = max(max_r_disp, norm(dr));
            }

            auto box_disp = l1_norm(box->cell - orig_box->cell);

            auto total_disp = 2.0f * max_r_disp + box_disp;
            if (total_disp > cell_pad)
                *invalid = true;
        }
    };
}