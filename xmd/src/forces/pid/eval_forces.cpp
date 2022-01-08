#include "forces/pid/eval_forces.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/files/csv.h>

namespace xmd::pid {

    void eval_pid_forces::operator()() {
        for (int idx = 0; idx < bundles->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_pid_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        auto const& pid_params = params["pseudo-improper dihedral"];

        auto& conf_ = ctx.ephemeral<conf_t>("pid_conf");

        auto lambda_ver = COSINE;
        auto lambda_ver_str = pid_params["lambda version"].as<std::string>();
        if (lambda_ver_str == "cosine")
            lambda_ver = COSINE;
        else if (lambda_ver_str == "algebraic")
            lambda_ver = ALGEBRAIC;
        else if (lambda_ver_str == "gaussian")
            lambda_ver = GAUSSIAN;

        auto const& bb_plus_params = pid_params["bb+"];
        conf_.bb_plus_lam.version() = lambda_ver;
        conf_.bb_plus_lam.alpha() = bb_plus_params["alpha"].as<quantity>();
        conf_.bb_plus_lam.psi_0() = bb_plus_params["psi_0"].as<quantity>();
        conf_.bb_plus_lj.r_min() = bb_plus_params["r_min"].as<quantity>();
        conf_.bb_plus_lj.depth() = bb_plus_params["depth"].as<quantity>();

        auto const& bb_minus_params = pid_params["bb-"];
        conf_.bb_minus_lam.version() = lambda_ver;
        conf_.bb_minus_lam.alpha() = bb_minus_params["alpha"].as<quantity>();
        conf_.bb_minus_lam.psi_0() = bb_minus_params["psi_0"].as<quantity>();
        conf_.bb_minus_lj.r_min() = bb_minus_params["r_min"].as<quantity>();
        conf_.bb_minus_lj.depth() = bb_minus_params["depth"].as<quantity>();

        auto& lj_variants_ = ctx.ephemeral<lj_variants>(
            "lj_variants");
        conf_.ss_ljs = lj_variants_.ss.data();

        auto const& ss_params = pid_params["ss"];
        conf_.ss_lam.psi_0() = ss_params["psi_0"].as<quantity>();
        conf_.ss_lam.alpha() = ss_params["alpha"].as<quantity>();

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.per_thread().var<vector<vec3r>>("F").data();
        box = &ctx.var<xmd::box>("box");
        bundles = &ctx.persistent<vector<pid_bundle>>(
            "pid_bundles");
        V = &ctx.per_thread().var<real>("V");
        prev = ctx.var<vector<int>>("prev").data();
        next = ctx.var<vector<int>>("next").data();
    }

    void eval_pid_forces::iter(int idx) const {
        real psi1, psi2;
        vec3r dpsi1_dr1p, dpsi1_dr1, dpsi1_dr1n, dpsi1_dr2;
        vec3r dpsi2_dr2p, dpsi2_dr2, dpsi2_dr2n, dpsi2_dr1;

        auto bundle = bundles->at(idx);
        auto i1 = bundle.i1(), i2 = bundle.i2();
        auto type = bundle.type();
        auto i1p = prev[i1], i1n = next[i1], i2p = prev[i2], i2n = next[i2];

        vec3r r1p = r[i1p], r1 = r[i1], r1n = r[i1n];
        vec3r r2p = r[i2p], r2 = r[i2], r2n = r[i2n];

        {
            auto &r1_ = r1p, &r2_ = r1, &r3_ = r1n, &r4_ = r2;
            auto r24 = box->r_uv(r2_, r4_);
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
            auto r24 = box->r_uv(r2_, r4_);
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

        auto r12 = box->r_uv(r1, r2);
        auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;

        real A = 0.0f, B = 0.0f, C = 0.0f, V_ = 0.0f;

        if (conf.bb_plus_lam.supp(psi1) && conf.bb_plus_lam.supp(psi2)) {
            auto [lam1, deriv1] = conf.bb_plus_lam(psi1);
            auto [lam2, deriv2] = conf.bb_plus_lam(psi2);
            auto [lj_V, lj_dV_dr] = conf.bb_plus_lj(r12_rn);

            V_ += lam1 * lam2 * lj_V;
            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }

        if (conf.bb_minus_lam.supp(psi1) && conf.bb_minus_lam.supp(psi2)) {
            auto [lam1, deriv1] = conf.bb_minus_lam(psi1);
            auto [lam2, deriv2] = conf.bb_minus_lam(psi2);
            auto [lj_V, lj_dV_dr] = conf.bb_minus_lj(r12_rn);

            V_ += lam1 * lam2 * lj_V;
            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }

        sink_lj ss_sink_lj = conf.ss_ljs[type];
        if (conf.ss_lam.supp(psi1) && conf.ss_lam.supp(psi2)) {
            auto [lam1, deriv1] = conf.ss_lam(psi1);
            auto [lam2, deriv2] = conf.ss_lam(psi2);
            auto [lj_V, lj_dV_dr] = ss_sink_lj(r12_n, r12_rn);

            V_ += lam1 * lam2 * lj_V;
            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }

        *V += V_;

        auto r12_u = r12 * r12_rn;
        F[i1p] -= A * dpsi1_dr1p;
        F[i1] -= A * dpsi1_dr1 + B * dpsi2_dr1 + C * r12_u;
        F[i1n] -= A * dpsi1_dr1n;
        F[i2p] -= B * dpsi2_dr2p;
        F[i2] -= A * dpsi1_dr2 + B * dpsi2_dr2 - C * r12_u;
        F[i2n] -= B * dpsi2_dr2n;
    }

    void eval_pid_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < bundles->size(); ++idx) {
            iter(idx);
        }
    }
}