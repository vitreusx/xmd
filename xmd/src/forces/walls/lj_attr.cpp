#include "forces/walls/lj_attr.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    void eval_lj_attr_wall_forces::loop(int idx) const {
        auto pair = pairs[idx];

        auto part_idx = pair.part_idx(), wall_idx = pair.wall_idx();
        auto status = pair.status();
        auto r_ = box->wrap(r[part_idx]);
        auto wall = walls[wall_idx];

        if (status == FREE) {
            if (abs(signed_dist(r_, wall)) < wall_min_dist) {
                pair.status() = FORMING_OR_FORMED;
                pair.joint_r() = project(r_, wall);
                pair.ref_t() = *t;
            }
        }
        else {
            auto joint_r = pair.joint_r();
            auto ref_t = pair.ref_t();
            auto saturation = min(*t - ref_t, cycle_time) * cycle_time_inv;
            if (status == BREAKING)
                saturation = (real)1.0 - saturation;

            auto r12 = joint_r - r_;
            auto r12_rn = norm_inv(r12);
            auto [V_, dV_dr] = lj(lj_depth, wall_min_dist)(r12_rn);
            *V += V_ * saturation;

            auto r12_u = r12 * r12_rn;
            auto f = saturation * dV_dr * r12_u;
            F[part_idx] += f;
            wall_F[wall_idx] -= f;

            if (status == FORMING_OR_FORMED && saturation == 1.0f) {
                if (factor * wall_min_dist * r12_rn < 1.0f) {
                    pair.status() = BREAKING;
                    pair.ref_t() = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
                pair.status() = FREE;
            }
        }
    }

    void eval_lj_attr_wall_forces::operator()() const {
        for (int idx = 0; idx < pairs.size(); ++idx) {
            loop(idx);
        }
    }

    void eval_lj_attr_wall_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < pairs.size(); ++idx) {
            loop(idx);
        }
    }
}