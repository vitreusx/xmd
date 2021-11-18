#pragma once

namespace xmd {
    void compute_simple_native_dihedrals::operator()() {
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

            auto diff = phi - d.native_phi;
            *V += 0.5f * cdh * diff * diff;
            auto dV_dphi = cdh * diff;

            auto r23_n = norm(r23);
            auto dphi_dr1 = -x12_23_u * r23_n * x12_23_rn;
            auto dphi_dr4 = x23_34_u * r23_n * x23_34_rn;
            auto df = (-dphi_dr1 * dot(r12, r23) + dphi_dr4 * dot(r23, r34)) /
                      (r23_n * r23_n);
            auto dphi_dr2 = -dphi_dr1 + df;
            auto dphi_dr3 = -dphi_dr4 - df;

            F[d.i1] -= dV_dphi * dphi_dr1;
            F[d.i2] -= dV_dphi * dphi_dr2;
            F[d.i3] -= dV_dphi * dphi_dr3;
            F[d.i4] -= dV_dphi * dphi_dr4;
        }
    }
}