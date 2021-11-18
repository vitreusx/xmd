#pragma once

namespace xmd {
    heurestic_dihedral_type::heurestic_dihedral_type(const amino_acid &a2,
        const amino_acid &a3) {

        auto code2 = (aa_code)a2, code3 = (aa_code)a3;
        char type2 = (code2 == GLY ? 0 : (code2 == PRO ? 1 : 2));
        char type3 = (code3 == GLY ? 0 : (code3 == PRO ? 1 : 2));
        code = static_cast<heurestic_dihedral_code>(3 * type2 + type3);
    }

    heurestic_dihedral_type::operator char() const {
        return static_cast<char>(code);
    }

    void compute_heurestic_dihedrals::operator()() {
        for (size_t i = 0; i < dihedrals.size(); ++i) {
            auto d = dihedrals[i];
            auto r1 = r[d.i1], r2 = r[d.i2], r3 = r[d.i3], r4 = r[d.i4];
            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto x12_23 = cross(r12, r23), x23_34 = cross(r23, r34);

            auto x12_23_rn = rnorm(x12_23), x23_34_rn = rnorm(x23_34);
            auto x12_23_u = x12_23 * x12_23_rn, x23_34_u = x23_34 * x23_34_rn;

            auto cos_phi = dot(x12_23_u, x23_34_u);
            auto phi = acosf(cos_phi);
            if (dot(x12_23, r34) < 0.0f) phi = -phi;
            auto sin_phi = sinf(phi);

            auto sin2_phi = sin_phi*sin_phi, cos2_phi = cos_phi*cos_phi,
                 sin_phi_cos_phi = sin_phi*cos_phi;

            *V += coeffs[0][d.type] + coeffs[1][d.type]*sin_phi +
                coeffs[2][d.type]*cos_phi + coeffs[3][d.type]*sin2_phi +
                coeffs[4][d.type]*cos2_phi + coeffs[5][d.type]*sin_phi_cos_phi;

            auto dV_dphi = coeffs[1][d.type]*cos_phi - coeffs[2][d.type]*sin_phi
                + 2.0f * (coeffs[3][d.type] + coeffs[4][d.type]) * sin_phi_cos_phi
                + coeffs[5][d.type] * (cos2_phi - sin2_phi);

            auto r23_n = norm(r23);
            auto dphi_dr1 = -x12_23_u * r23_n * x12_23_rn;
            auto dphi_dr4 = x23_34_u * r23_n * x23_34_rn;
            auto df = (-dphi_dr1*dot(r12, r23)+dphi_dr4*dot(r23, r34))/(r23_n*r23_n);
            auto dphi_dr2 = -dphi_dr1 + df;
            auto dphi_dr3 = -dphi_dr4 - df;

            F[d.i1] -= dV_dphi * dphi_dr1;
            F[d.i2] -= dV_dphi * dphi_dr2;
            F[d.i3] -= dV_dphi * dphi_dr3;
            F[d.i4] -= dV_dphi * dphi_dr4;
        }
    }
}