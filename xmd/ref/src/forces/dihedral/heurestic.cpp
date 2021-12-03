#include "forces/dihedral/heurestic.h"

namespace xmd {

    heurestic_dihedral_type::heurestic_dihedral_type(const amino_acid &a2,
        const amino_acid &a3) {
        auto code2 = (aa_code)a2, code3 = (aa_code)a3;
        int8_t type2 = (code2 == GLY ? (int8_t)0 : (code2 == PRO ? (int8_t)1 : (int8_t)2));
        int8_t type3 = (code3 == GLY ? (int8_t)0 : (code3 == PRO ? (int8_t)1 : (int8_t)2));
        val = (int8_t)3 * type2 + type3;
    }

    constexpr heurestic_dihedral_type::operator int8_t() {
        return val;
    }

    constexpr heurestic_dihedral_type::heurestic_dihedral_type(int8_t val):
        val{val} {};

    void eval_heurestic_dihedral_forces::operator()() const {
        for (int idx = 0; idx < dihedrals.size; ++idx) {
            auto i1 = dihedrals.i1[idx], i2 = dihedrals.i2[idx],
                i3 = dihedrals.i3[idx], i4 = dihedrals.i4[idx];
            auto type_val = (int8_t)dihedrals.type[idx];

            auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto x12_23 = cross(r12, r23), x23_34 = cross(r23, r34);

            auto x12_23_rn = norm_inv(x12_23), x23_34_rn = norm_inv(x23_34);
            auto x12_23_u = x12_23 * x12_23_rn, x23_34_u = x23_34 * x23_34_rn;

            auto cos_phi = dot(x12_23_u, x23_34_u);
            auto phi = cos(cos_phi);
            if (dot(x12_23, r34) < 0.0f) phi = -phi;
            auto sin_phi = sin(phi);

            auto sin2_phi = sin_phi*sin_phi, cos2_phi = cos_phi*cos_phi,
                sin_phi_cos_phi = sin_phi*cos_phi;

            *V += coeffs[0][type_val] + coeffs[1][type_val]*sin_phi +
                  coeffs[2][type_val]*cos_phi + coeffs[3][type_val]*sin2_phi +
                  coeffs[4][type_val]*cos2_phi + coeffs[5][type_val]*sin_phi_cos_phi;

            auto dV_dphi = coeffs[1][type_val]*cos_phi - coeffs[2][type_val]*sin_phi
                           + 2.0f * (coeffs[3][type_val] + coeffs[4][type_val]) * sin_phi_cos_phi
                           + coeffs[5][type_val] * (cos2_phi - sin2_phi);

            auto r23_n = norm(r23);
            auto dphi_dr1 = -x12_23_u * r23_n * x12_23_rn;
            auto dphi_dr4 = x23_34_u * r23_n * x23_34_rn;
            auto df = (-dphi_dr1*dot(r12, r23)+dphi_dr4*dot(r23, r34))/(r23_n*r23_n);
            auto dphi_dr2 = -dphi_dr1 + df;
            auto dphi_dr3 = -dphi_dr4 - df;

            F[i1] -= dV_dphi * dphi_dr1;
            F[i2] -= dV_dphi * dphi_dr2;
            F[i3] -= dV_dphi * dphi_dr3;
            F[i4] -= dV_dphi * dphi_dr4;
        }
    }
}