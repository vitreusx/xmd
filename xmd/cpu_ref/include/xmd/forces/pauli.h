#pragma once
#include <xmd/types/vec3_array.h>
#include <xmd/model/box.h>
#include <xmd/math.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    struct pauli_pair_array {
        int *i1, *i2;
        int size;
    };

    class eval_pauli_exclusion_forces {
    public:
        float depth, r_excl;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        pauli_pair_array pairs;
        float *V;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < pairs.size; ++idx) {
                auto i1 = pairs.i1[idx], i2 = pairs.i2[idx];
                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_rn = norm_inv(r12);

                auto within = (1.0f < r12_rn * r_excl);
                if (within) {
                    auto r12_u = r12 * r12_rn;
                    auto [V_, dV_dr] = lj(depth, r_excl)(r12_rn);

                    *V += V_;
                    F[i1] -= r12_u * dV_dr;
                    F[i2] += r12_u * dV_dr;
                }
            }
        }
    };
}
