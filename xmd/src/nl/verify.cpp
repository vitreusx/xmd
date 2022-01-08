#include "nl/verify.h"

namespace xmd::nl {
    void verify::operator()() const {
        if (*first_time) {
            *first_time = false;
            *invalid = true;
        }
        else {
            real max_r_disp = 0.0f;
//#pragma omp taskloop default(none) reduction(max:max_r_disp)
            for (int idx = 0; idx < num_particles; ++idx) {
                auto r_ = r[idx], orig_r_ = data->orig_r[idx];
                auto dr = box->r_uv(r_, orig_r_);
                max_r_disp = max(max_r_disp, norm(dr));
            }

            auto box_disp = l1_norm(box->cell - data->orig_box.cell);

            real total_disp = 2.0f * max_r_disp + box_disp;
            if (total_disp > data->orig_pad)
                *invalid = true;
        }
    }

    void verify::declare_vars(context& ctx) {
        r = ctx.var<vector<vec3r>>("r").data();
        data = &ctx.var<nl_data>("nl_data");
        box = &ctx.var<xmd::box>("box");
        invalid = &ctx.var<bool>("invalid");
        first_time = &ctx.ephemeral<bool>("nl_first_time", true);
        num_particles = ctx.var<int>("num_particles");
    }
}