#pragma once
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/model/box.h>

namespace xmd {
    struct native_ssbond_span {
        array<int> cys_i1, cys_i2;
        int size;
    };

    class eval_native_disulfide_forces {
    public:
        float H1, nat_r;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        native_ssbond_span bonds;
        float *V;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < bonds.size; ++idx) {
                auto cys_i1 = bonds.cys_i1[idx], cys_i2 = bonds.cys_i2[idx];
                auto r1 = r[cys_i1], r2 = r[cys_i2];
                auto r12 = box->ray(r1, r2);

                auto r12_n = norm(r12);
                auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

                auto r12_u = r12 / r12_n;
                *V += V_;
                F[cys_i1] -= dV_dr * r12_u;
                F[cys_i2] += dV_dr * r12_u;
            }
        }
    };
}