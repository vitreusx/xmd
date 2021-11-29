#pragma once
#include <tuple>
#include <xmd/math.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd {
    enum lambda_version {
        COSINE, ALGEBRAIC, GAUSSIAN
    };

    class lambda_func {
    public:
        float psi_0, alpha;
        lambda_version version;

        lambda_func() = default;

        lambda_func(float psi_0, float alpha, bool cosine_version):
            psi_0{psi_0}, alpha{alpha}, cosine_version{cosine_version} {};

        inline bool supp(float psi) const {
            return abs(alpha * (psi - psi_0)) < M_PI;
        }

        inline std::tuple<float, float> operator()(float psi) const {
            switch (version) {
            case COSINE:
                auto s = alpha * (psi - psi_0);
                auto val = 0.5f * cos(s) + 0.5f;
                auto deriv = -0.5f * alpha * sin(s);
                return std::make_tuple(val, deriv);
            case ALGEBRAIC:
                auto s = alpha * (psi - psi_0);
                auto t = abs(s / M_PI);
                auto x_inv = 1.0f/(2.0f*t*t-2.0f*t-1);
                auto val = (t*t-2.0f*t+1.0f)*x_inv;
                auto deriv = (2.0f*t*(t-1.0f))*x_inv*x_inv / M_PI;
                deriv *= (s < 0.0f ? -1.0f : 1.0f);
                return std::make_tuple(val, deriv);
            default:
                // TODO: implement the Gaussian version
                return std::make_tuple(0.0f, 0.0f);
            }
        }
    };

    struct lambda_func_array {
        float *psi_0, *alpha;
        lambda_version version;

        inline lambda_func operator[](int idx) const {
            return { psi_0[idx], alpha[idx], version };
        }
    };

    struct pid_bundle_span {
        array<int> i1p, i1, i1n;
        array<int> i2p, i2, i2n;
        array<int8_t> type;
        int size;
    };

    class eval_pid_forces {
    public:
        lambda_func bb_plus_lam, bb_minus_lam;
        lj bb_plus_lj, bb_minus_lj;

        lambda_func_array ss_lams;
        sink_lj_array ss_sink_ljs;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        pid_bundle_span bundles;
        float *V;

    public:
        inline void operator()() {
            for (int idx = 0; idx < bundles.size; ++idx) {
                float psi1, psi2;
                vec3f dpsi1_dr1p, dpsi1_dr1, dpsi1_dr1n, dpsi1_dr2;
                vec3f dpsi2_dr2p, dpsi2_dr2, dpsi2_dr2n, dpsi2_dr1;

                auto i1p = bundles.i1p[idx], i1 = bundles.i1[idx], i1n = bundles.i1n[idx];
                auto i2p = bundles.i2p[idx], i2 = bundles.i2[idx], i2n = bundles.i2n[idx];
                auto type = bundles.type[idx];

                auto r1p = r[i1p], r1 = r[i1], r1n = r[i1n];
                auto r2p = r[i2p], r2 = r[i2], r2n = r[i2n];

                {
                    auto &r1_ = r1p, &r2_ = r1, &r3_ = r1n, &r4_ = r2;
                    auto r24 = box->ray(r2_, r4_);
                    auto r12 = r2_ - r1_;
                    auto r23 = r3_ - r2_;
                    auto r13 = r3_ - r1_;
                    auto r14 = r12 + r24;

                    auto rij = -r23;
                    auto rkj = -r13;
                    auto rkl = -r14;
                    auto rm = cross(r23, r12);
                    auto rn = cross(r13, r14);
                    auto rm_rn = norm_inv(rm);
                    auto rn_rn = norm_inv(rn);
                    auto rkj_n = norm(rkj);

                    auto fi = rm * rkj_n * rm_rn;
                    auto fl = - rn * rkj_n * rn_rn;
                    auto df = (fi * dot(rij, rkj) - fl * dot(rkl, rkj)) / (rkj_n * rkj_n);
                    auto fj = -fi + df;
                    auto fk = -fl - df;

                    dpsi1_dr1p = fi;
                    dpsi1_dr1 = fj;
                    dpsi1_dr1n = fk;
                    dpsi1_dr2 = fl;

                    auto cos_psi1 = dot(rm, rn) * rm_rn * rn_rn;
                    psi1 = acos(cos_psi1);
                    if (dot(rij, rn) < 0.0f) psi1 = -psi1;
                }

                {
                    auto &r1_ = r2p, &r2_ = r2, &r3_ = r2n, &r4_ = r1;
                    auto r24 = box->ray(r2_, r4_);
                    auto r12 = r2_ - r1_;
                    auto r23 = r3_ - r2_;
                    auto r13 = r3_ - r1_;
                    auto r14 = r12 + r24;

                    auto rij = -r23;
                    auto rkj = -r13;
                    auto rkl = -r14;
                    auto rm = cross(r23, r12);
                    auto rn = cross(r13, r14);
                    auto rm_rn = norm_inv(rm);
                    auto rn_rn = norm_inv(rn);
                    auto rkj_n = norm(rkj);

                    auto fi = rm * rkj_n * rm_rn;
                    auto fl = - rn * rkj_n * rn_rn;
                    auto df = (fi * dot(rij, rkj) - fl * dot(rkl, rkj)) / (rkj_n * rkj_n);
                    auto fj = -fi + df;
                    auto fk = -fl - df;

                    dpsi1_dr1p = fi;
                    dpsi1_dr1 = fj;
                    dpsi1_dr1n = fk;
                    dpsi1_dr2 = fl;

                    auto cos_psi2 = dot(rm, rn) * rm_rn * rn_rn;
                    psi2 = acos(cos_psi2);
                    if (dot(rij, rn) < 0.0f) psi2 = -psi2;
                }

                auto r12 = box->ray(r1, r2);
                auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;

                float A = 0.0f, B = 0.0f, C = 0.0f, V_ = 0.0f;

                if (bb_plus_lam.supp(psi1) && bb_plus_lam.supp(psi2)) {
                    auto [lam1, deriv1] = bb_plus_lam(psi1);
                    auto [lam2, deriv2] = bb_plus_lam(psi2);
                    auto [lj_V, lj_dV_dr] = bb_plus_lj(r12_rn);

                    V_ += lam1 * lam2 * lj_V;
                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                if (bb_minus_lam.supp(psi1) && bb_minus_lam.supp(psi2)) {
                    auto [lam1, deriv1] = bb_minus_lam(psi1);
                    auto [lam2, deriv2] = bb_minus_lam(psi2);
                    auto [lj_V, lj_dV_dr] = bb_minus_lj(r12_rn);

                    V_ += lam1 * lam2 * lj_V;
                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                auto ss_lam = ss_lams[type];
                auto ss_sink_lj = ss_sink_ljs[type];
                if (ss_lam.supp(psi1) && ss_lam.supp(psi2)) {
                    auto [lam1, deriv1] = ss_lam(psi1);
                    auto [lam2, deriv2] = ss_lam(psi2);
                    auto [lj_V, lj_dV_dr] = ss_sink_lj(r12_n, r12_rn);

                    V_ += lam1 * lam2 * lj_V;
                    A += deriv1 * lam2 * lj_V;
                    B += deriv2 * lam1 * lj_V;
                    C += lam1 * lam2 * lj_dV_dr;
                }

                auto r12_u = r12 * r12_rn;
                *V += V_;
                F[i1p] -= A * dpsi1_dr1p;
                // TODO: check if it should be `+` before C
                F[i1] -= A * dpsi1_dr1 + B * dpsi2_dr1 + C * r12_u;
                F[i1n] -= A * dpsi1_dr1n;
                F[i2p] -= B * dpsi2_dr2p;
                // TODO: (as before)
                F[i2] -= A * dpsi1_dr2 + B * dpsi2_dr2 - C * r12_u;
                F[i2n] -= B * dpsi2_dr2n;
            }
        }
    };
}
