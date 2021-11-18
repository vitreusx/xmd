#pragma once

namespace xmd {
    void compute_native_angles::operator()() {
        for (size_t i = 0; i < angles.size(); ++i) {
            const_ref<native_angle> a = angles[i];
            const_ref<vec3f> r1 = r[a.i1], r2 = r[a.i2], r3 = r[a.i3];
            auto r12 = r2 - r1, r23 = r3 - r2;

            auto x12_23 = cross(r12, r23);
            auto r12_rn = rnorm(r12), r23_rn = rnorm(r23);

            auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
            auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
            auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

            auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
            auto theta = acosf(cos_theta);
            auto dtheta = theta - a.native_theta;

            *V += k * dtheta * dtheta;

            auto dV_dtheta = 2.0f * k * dtheta;
            F[a.i1] -= dV_dtheta * dtheta_dr1;
            F[a.i2] -= dV_dtheta * dtheta_dr2;
            F[a.i3] -= dV_dtheta * dtheta_dr3;
        }
    }
}