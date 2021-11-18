#pragma once

namespace xmd {
    void compute_heurestic_angles::operator()() {
        for (size_t i = 0; i < angles.size(); ++i) {
            const_ref<heurestic_angle> a = angles[i];
            const_ref<vec3f> r1 = r[a.i1], r2 = r[a.i2], r3 = r[a.i3];
            auto r12 = r2 - r1, r23 = r3 - r2;

            auto x12_23 = cross(r12, r23);
            auto r12_rn = rnorm(r12), r23_rn = rnorm(r23);

            auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
            auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
            auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

            auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
            auto theta = acosf(cos_theta);

            float angle_V = 0.0f, dV_dtheta = 0.0f;
            for (int d = POLY_DEG; d >= 0; --d) {
                auto coeff = poly_coeffs[d][a.type];
                if (d > 0) dV_dtheta = d * coeff + theta * dV_dtheta;
                angle_V = coeff + theta * angle_V;
            }

            *V += angle_V;
            F[a.i1] -= dV_dtheta * dtheta_dr1;
            F[a.i2] -= dV_dtheta * dtheta_dr2;
            F[a.i3] -= dV_dtheta * dtheta_dr3;
        }
    }
}