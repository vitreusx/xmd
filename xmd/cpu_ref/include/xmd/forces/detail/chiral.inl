#pragma once

namespace xmd {
    chiral_quad compute_chiral_forces::make_quad(int i1, int i2, int i3, int i4) {
        auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
        auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;

        chiral_quad q;
        q.i1 = i1;
        q.i2 = i2;
        q.i3 = i3;
        q.i4 = i4;
        q.native_d0_rncube = powf(rnorm(r23), 3.0f);
        q.native_chirality = dot(r12, cross(r23, r34)) * q.native_d0_rncube;

        return q;
    }

    void compute_chiral_forces::operator()() {
        for (size_t i = 0; i < quads.size(); ++i) {
            auto q = quads[i];
            auto r1 = r[q.i1], r2 = r[q.i2], r3 = r[q.i3], r4 = r[q.i4];
            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto x12_23 = cross(r12, r23), x12_34 = cross(r12, r34),
                 x23_34 = cross(r23, r34);

            auto chirality = dot(r12, x23_34) * q.native_d0_rncube;
            auto d_chirality = chirality - q.native_chirality;

            *V += 0.5f * e_chi * d_chirality * d_chirality;

            auto f = e_chi * d_chirality * q.native_d0_rncube;
            F[q.i1] += f * x12_23;
            F[q.i2] -= f * (x12_34 + x23_34);
            F[q.i3] += f * (x12_23 + x12_34);
            F[q.i4] -= f * x12_23;
        }
    }
}