#pragma once
#include <xmd/types/vec3_array.h>
#include <xmd/model/box.h>
#include <xmd/math.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    struct go_contact_span {
        array<int> i1, i2;
        array<float> nat_dist;
        int size;
    };

    class eval_go_forces {
    public:
        float depth;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        go_contact_span contacts;
        float *V;

    public:
        inline void operator()() {
            for (int idx = 0; idx < contacts.size; ++idx) {
                auto i1 = contacts.i1[idx], i2 = contacts.i2[idx];
                auto nat_dist = contacts.nat_dist[idx];

                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_rn = norm_inv(r12);

                auto r12_u = r12 * r12_rn;
                auto [V_, dV_dr] = lj(depth, nat_dist)(r12_rn);

                *V += V_;
                F[i1] -= r12_u * dV_dr;
                F[i2] += r12_u * dV_dr;
            }
        }
    };
}
