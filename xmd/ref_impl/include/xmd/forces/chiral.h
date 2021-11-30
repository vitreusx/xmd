#pragma once
#include <xmd/types/vec3_array.h>
#include <xmd/math.h>

namespace xmd {
    struct chiral_quad_span {
        array<int> i1, i2, i3, i4;
        array<float> nat_chir, nat_factor;
        int size;
    };

    class eval_chiral_forces {
    public:
        float e_chi;

    public:
        vec3f_array r, F;
        chiral_quad_span quads;
        float *V;

    public:
        inline void derive_quads(int *i1, int *i2, int *i3, int *i4, int size) {
            quads.i1 = i1;
            quads.i2 = i2;
            quads.i3 = i3;
            quads.i4 = i4;
            quads.size = size;

            for (int idx = 0; idx < size; ++idx) {
                auto r1 = r[i1[idx]], r2 = r[i2[idx]], r3 = r[i3[idx]],
                    r4 = r[i4[idx]];
                auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;

                auto nat_factor = ipow<3>(norm_inv(r23));
                auto nat_chir = dot(r12, cross(r23, r34)) * nat_factor;

                quads.nat_factor[idx] = nat_factor;
                quads.nat_chir[idx] = nat_chir;
            }
        }

    public:
        inline void operator()() const {
            for (int idx = 0; idx < quads.size; ++idx) {
                auto i1 = quads.i1[idx], i2 = quads.i2[idx], i3 = quads.i3[idx],
                    i4 = quads.i4[idx];
                auto nat_chir = quads.nat_chir[idx];
                auto nat_factor = quads.nat_factor[idx];

                auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
                auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
                auto x12_23 = cross(r12, r23), x12_34 = cross(r12, r34),
                    x23_34 = cross(r23, r34);

                auto chir = dot(r12, x23_34) * nat_factor;
                auto chir_diff = chir - nat_chir;

                *V += 0.5f * e_chi * chir_diff * chir_diff;

                auto f = e_chi * chir_diff * nat_factor;
                F[i1] += f * x12_23;
                F[i2] -= f * (x12_34 + x23_34);
                F[i3] += f * (x12_23 + x12_34);
                F[i4] -= f * x12_23;
            }
        }
    };
}
