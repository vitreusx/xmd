#pragma once

namespace xmd {
    template<typename Functor, bool CosineVersion>
    bool gen_lambda_func<Functor, CosineVersion>::supports(float psi) const {
        return fabsf(alpha * (psi - psi_0)) < M_PI;
    }

    template<typename Functor, bool CosineVersion>
    std::tuple<float, float>
    gen_lambda_func<Functor, CosineVersion>::operator()(float psi) const {
        auto s = alpha * (psi - psi_0);
        if constexpr (CosineVersion) {
            auto val = 0.5f * cosf(s) + 0.5f;
            auto deriv = -0.5f * alpha * sinf(s);
            return std::make_tuple(val, deriv);
        }
        else {
            auto t = fabsf(s / M_PI);
            auto x_inv = 1.0f/(2.0f*t*t-2.0f*t-1);
            auto val = (t*t-2.0f*t+1.0f)*x_inv;
            auto deriv = (2.0f*t*(t-1.0f))*x_inv*x_inv / M_PI;
            deriv *= (s < 0.0f ? -1.0f : 1.0f);
            return std::make_tuple(val, deriv);
        }
    }

    void compute_pid::per_bb_term(lj const& lj, lambda_func<> const& lam, float psi[2],
            float r_inv, float& A, float& B, float& C) const {
        if (lam.supports(psi[0]) && lam.supports(psi[1])) {
            auto [lam1, deriv1] = lam(psi[0]);
            auto [lam2, deriv2] = lam(psi[1]);
            auto [lj_V, lj_dV_dr] = lj(r_inv);

            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }
    }

    void compute_pid::per_ss_term(sink_lj const& lj, lambda_func<> const& lam, float psi[2],
        float r, float r_inv, float& A, float& B, float& C) const {
        if (lam.supports(psi[0]) && lam.supports(psi[1])) {
            auto [lam1, deriv1] = lam(psi[0]);
            auto [lam2, deriv2] = lam(psi[1]);
            auto [lj_V, lj_dV_dr] = lj(r, r_inv);

            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }
    }

    void compute_pid::operator()() {
        for (size_t i = 0; i < bundles.size(); ++i) {
            const_ref<pid_bundle> bundle = bundles[i];

            float psi[2];
            vec3f dpsi_dr[2][6];

            int idx[6] = {
                bundle.i1p, bundle.i1, bundle.i1n,
                bundle.i2p, bundle.i2, bundle.i2n
            };

            int pass_idx[2][4] = {
                {0, 1, 2, 4},
                {3, 4, 5, 1}
            };

            for (size_t pass = 0; pass < 2; ++pass) {
                auto loc1 = pass_idx[pass][0], loc2 = pass_idx[pass][1],
                     loc3 = pass_idx[pass][2], loc4 = pass_idx[pass][3];

                auto i1 = idx[loc1], i2 = idx[loc2], i3 = idx[loc3], i4 = idx[loc4];

                auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
                auto r24 = box->uv(r2, r4);
                auto r12 = r2 - r1, r23 = r3 - r2, r13 = r3 - r1, r14 = r12 + r24;

                auto rij = -r23, rkj = -r13, rkl = -r14, rm = cross(r23, r12),
                     rn = cross(r13, r14);
                auto rm_rn = rnorm(rm), rn_rn = rnorm(rn), rkj_n = norm(rkj);

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
                auto psi_pass = acosf(cos_psi);
                if (dot(rij, rn) < 0.0f) psi_pass = -psi_pass;
                psi[pass] = psi_pass;
            }

            auto r1 = r[bundle.i1], r2 = r[bundle.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;

            float A = 0.0f, B = 0.0f, C = 0.0f;
            per_bb_term(bb_plus_lj, bb_plus_lam, psi, r12_rn, A, B, C);
            per_bb_term(bb_minus_lj, bb_minus_lam, psi, r12_rn, A, B, C);
            per_ss_term(ss_sink_ljs[bundle.type], ss_lams[bundle.type],
                psi, r12_n, r12_rn, A, B, C);

            for (size_t j = 0; j < 6; ++j) {
                F[idx[j]] -= A * dpsi_dr[0][j];
                F[idx[j]] -= B * dpsi_dr[1][j];
            }

            auto r12_u = r12 * r12_rn;
            F[bundle.i1] += C * r12_u;
            F[bundle.i2] -= C * r12_u;
        }
    }
}