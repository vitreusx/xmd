#pragma once
#include <tuple>
#include <xmd/math.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>
#include <xmd/types/vec3_array.h>
#include <xmd/model/box.h>

namespace xmd {
    class lambda_func {
    public:
        float psi_0, alpha;
        bool cosine_version;

        lambda_func() = default;

        lambda_func(float psi_0, float alpha, bool cosine_version):
            psi_0{psi_0}, alpha{alpha}, cosine_version{cosine_version} {};

        inline bool supp(float psi) const {
            return abs(alpha * (psi - psi_0)) < M_PI;
        }

        inline std::tuple<float, float> operator()(float psi) const {
            auto s = alpha * (psi - psi_0);
            if (cosine_version) {
                auto val = 0.5f * cos(s) + 0.5f;
                auto deriv = -0.5f * alpha * sin(s);
                return std::make_tuple(val, deriv);
            }
            else {
                auto t = abs(s / M_PI);
                auto x_inv = 1.0f/(2.0f*t*t-2.0f*t-1);
                auto val = (t*t-2.0f*t+1.0f)*x_inv;
                auto deriv = (2.0f*t*(t-1.0f))*x_inv*x_inv / M_PI;
                deriv *= (s < 0.0f ? -1.0f : 1.0f);
                return std::make_tuple(val, deriv);
            }
        }
    };

    struct lambda_func_array {
        float *psi_0, *alpha;
        bool cosine_version;

        inline lambda_func operator[](int idx) const {
            return { psi_0[idx], alpha[idx], cosine_version };
        }
    };

    struct pid_bundle_array {
        int *i1p, *i1, *i1n;
        int *i2p, *i2, *i2n;
        int8_t *type;
        int size;
    };

    class eval_pid_forces {
    public:
        lambda_func bb_plus_lam, bb_minus_lam;
        lj bb_plus_lj, bb_minus_lj;

        lambda_func_array ss_lams;
        sink_lj_array ss_sink_ljs;

    public:
        vec3f_array  r, F;
        box<vec3f> *box;
        pid_bundle_array bundles;
        float *V;

    public:
        inline void operator()() const {
            int *indices[6] = {
                bundles.i1p, bundles.i1, bundles.i1n,
                bundles.i2p, bundles.i2, bundles.i2n
            };

            int local[2][4] = {
                { 0, 1, 2, 4 },
                { 3, 4, 5, 1 }
            };

            for (int idx = 0; idx < bundles.size; ++idx) {
                float psi[2];
                vec3f dpsi_dr[2][6];

                for (int pass = 0; pass < 2; ++pass) {
                    auto loc1 = local[pass][0], i1 = indices[loc1][idx];
                    auto loc2 = local[pass][1], i2 = indices[loc2][idx];
                    auto loc3 = local[pass][2], i3 = indices[loc3][idx];
                    auto loc4 = local[pass][3], i4 = indices[loc4][idx];

                    auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
                    auto r24 = box->ray(r2, r4);
                    auto r12 = r2 - r1, r23 = r3 - r2, r13 = r3 - r1, r14 = r12 + r24;

                    auto rij = -r23, rkj = -r13, rkl = -r14, rm = cross(r23, r12),
                        rn = cross(r13, r14);
                    auto rm_rn = norm_inv(rm), rn_rn = norm_inv(rn), rkj_n = norm(rkj);

                    auto fi = rm * rkj_n * rm_rn;
                    auto fl = - rn * rkj_n * rn_rn;
                    auto df = (fi * dot(rij, rkj) - fl * dot(rkl, rkj)) / (rkj_n * rkj_n);
                    auto fj = -fi + df;
                    auto fk = -fl - df;

                    dpsi_dr[pass][loc1] = fi;
                    dpsi_dr[pass][loc2] = fj;
                    dpsi_dr[pass][loc3] = fk;
                    dpsi_dr[pass][loc4] = fl;

                    auto cos_psi = dot(rm, rn) * rm_rn * rn_rn;
                    auto psi_pass = acos(cos_psi);
                    if (dot(rij, rn) < 0.0f) psi_pass = -psi_pass;
                    psi[pass] = psi_pass;
                }

                auto i1 = bundles.i1[idx], i2 = bundles.i2[idx];
                auto type = bundles.type[idx];

                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;

                float A = 0.0f, B = 0.0f, C = 0.0f;

                if (bb_plus_lam.supp(psi[0]) && bb_plus_lam.supp(psi[1])) {
                    auto [lam1, deriv1] = bb_plus_lam(psi[0]);
                    auto [lam2, deriv2] = bb_plus_lam(psi[1]);
                    auto [lj_V, lj_dV_dr] = bb_plus_lj(r12_rn);

                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                if (bb_minus_lam.supp(psi[0]) && bb_minus_lam.supp(psi[1])) {
                    auto [lam1, deriv1] = bb_minus_lam(psi[0]);
                    auto [lam2, deriv2] = bb_minus_lam(psi[1]);
                    auto [lj_V, lj_dV_dr] = bb_minus_lj(r12_rn);

                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                auto ss_lam = ss_lams[type];
                auto ss_sink_lj = ss_sink_ljs[type];
                if (ss_lam.supp(psi[0]) && ss_lam.supp(psi[1])) {
                    auto [lam1, deriv1] = ss_lam(psi[0]);
                    auto [lam2, deriv2] = ss_lam(psi[1]);
                    auto [lj_V, lj_dV_dr] = ss_sink_lj(r12_n, r12_rn);

                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                for (size_t j = 0; j < 6; ++j) {
                    F[indices[j][idx]] -= A * dpsi_dr[0][j];
                    F[indices[j][idx]] -= B * dpsi_dr[1][j];
                }

                auto r12_u = r12 * r12_rn;
                F[i1] += C * r12_u;
                F[i2] -= C * r12_u;
            }
        }
    };
}
