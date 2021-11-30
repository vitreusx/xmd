#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/types/amino_acid.h>

namespace xmd {
    struct native_angle_span {
        array<int> i1, i2, i3;
        array<float> nat_theta;
        int size;
    };

    class eval_native_angle_forces {
    public:
        float k;

    public:
        float *V;
        vec3f_array r, F;
        native_angle_span angles;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < angles.size; ++idx) {
                int i1 = angles.i1[idx], i2 = angles.i2[idx], i3 = angles.i3[idx];
                auto nat_theta = angles.nat_theta[idx];
                auto r1 = r[i1], r2 = r[i2], r3 = r[i3];

                auto r12 = r2 - r1, r23 = r3 - r2;

                auto x12_23 = cross(r12, r23);
                auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);

                auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
                auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
                auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

                auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
                auto theta = acos(cos_theta);
                auto dtheta = theta - nat_theta;

                *V += k * dtheta * dtheta;

                auto dV_dtheta = 2.0f * k * dtheta;
                F[i1] -= dV_dtheta * dtheta_dr1;
                F[i2] -= dV_dtheta * dtheta_dr2;
                F[i3] -= dV_dtheta * dtheta_dr3;
            }
        }
    };
}
