#include "forces/angle/native.h"
#include <xmd/model/model.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_native_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_native_angle_forces::iter(int idx) const {
        auto angle = angles[idx];

        int i1 = angle.i1(), i2 = angle.i2(), i3 = angle.i3();
        auto nat_theta = angle.nat_theta();
        auto r1 = r[i1], r2 = r[i2], r3 = r[i3];

        auto r12 = r2 - r1, r23 = r3 - r2;

        auto x12_23 = cross(r12, r23);
        auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);

        auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
        auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
        auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

        auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
        auto theta = acos(cos_theta);

        auto dtheta = theta - nat_theta;
        *V += (real)0.5 * k * dtheta * dtheta;

        auto dV_dtheta = k * dtheta;
        F[i1] -= dV_dtheta * dtheta_dr1;
        F[i2] -= dV_dtheta * dtheta_dr2;
        F[i3] -= dV_dtheta * dtheta_dr3;
    }

    void eval_native_angle_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }
}