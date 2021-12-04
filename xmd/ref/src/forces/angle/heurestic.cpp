#include "forces/angle/heurestic.h"

namespace xmd {

    heurestic_angle_type::heurestic_angle_type(const amino_acid &a1,
        const amino_acid &a2) {
        auto code1 = (aa_code)a1, code2 = (aa_code)a2;
        int8_t type1 = (code1 == GLY ? (int8_t)0 : (code1 == PRO ? (int8_t)1 : (int8_t)2));
        int8_t type2 = (code2 == GLY ? (int8_t)0 : (code2 == PRO ? (int8_t)1 : (int8_t)2));
        val = (int8_t)3 * type1 + type2;
    }

    constexpr heurestic_angle_type::operator int8_t() {
        return val;
    }

    constexpr heurestic_angle_type::heurestic_angle_type(int8_t val) :
        val{val} {};

    void eval_heurestic_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size; ++idx) {
            auto i1 = angles.i1[idx], i2 = angles.i2[idx], i3 = angles.i3[idx];
            auto type_val = (int8_t)angles.type[idx];

            auto r1 = r[i1], r2 = r[i2], r3 = r[i3];
            auto r12 = r2 - r1, r23 = r3 - r2;

            auto x12_23 = cross(r12, r23);
            auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);

            auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
            auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
            auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

            auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
            auto theta = acos(cos_theta);

            real angle_V = 0.0f, dV_dtheta = 0.0f;
            for (int d = POLY_DEG; d >= 0; --d) {
                auto coeff = poly_coeffs[d][type_val];
                if (d > 0) dV_dtheta = (real)d * coeff + theta * dV_dtheta;
                angle_V = coeff + theta * angle_V;
            }

            *V += angle_V;
            F[i1] -= dV_dtheta * dtheta_dr1;
            F[i2] -= dV_dtheta * dtheta_dr2;
            F[i3] -= dV_dtheta * dtheta_dr3;
        }
    }
}