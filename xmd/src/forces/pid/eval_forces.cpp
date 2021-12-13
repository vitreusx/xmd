#include "forces/pid/eval_forces.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/files/csv.h>

namespace xmd::pid {
    void eval_pid_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx)
            loop_iter(idx);
    }

    void eval_pid_forces::init_from_vm(vm &vm_inst) {
        conf = vm_inst.find_or<conf_t>("pid_conf", [&]() -> auto& {
            auto& params = vm_inst.find<param_file>("params");
            auto const& pid_params = params["pseudo-improper dihedral"];

            auto& conf_ = vm_inst.emplace<conf_t>("pid_conf");

            auto lambda_ver = COSINE;
            auto lambda_ver_str = pid_params["lambda version"].as<std::string>();
            if (lambda_ver_str == "cosine")
                lambda_ver = COSINE;
            else if (lambda_ver_str == "algebraic")
                lambda_ver = ALGEBRAIC;
            else if (lambda_ver_str == "gaussian")
                lambda_ver = GAUSSIAN;

            auto const& bb_plus_params = pid_params["bb+"];
            conf_.bb_plus_lam.version = lambda_ver;
            conf_.bb_plus_lam.alpha = bb_plus_params["alpha"].as<quantity>();
            conf_.bb_plus_lam.psi_0 = bb_plus_params["psi_0"].as<quantity>();
            conf_.bb_plus_lj.r_min = bb_plus_params["r_min"].as<quantity>();
            conf_.bb_plus_lj.depth = bb_plus_params["depth"].as<quantity>();

            auto const& bb_minus_params = pid_params["bb-"];
            conf_.bb_minus_lam.version = lambda_ver;
            conf_.bb_minus_lam.alpha = bb_minus_params["alpha"].as<quantity>();
            conf_.bb_minus_lam.psi_0 = bb_minus_params["psi_0"].as<quantity>();
            conf_.bb_minus_lj.r_min = bb_minus_params["r_min"].as<quantity>();
            conf_.bb_minus_lj.depth = bb_minus_params["depth"].as<quantity>();

            auto& lj_variants_ = vm_inst.find_or_emplace<lj_variants>(
                "lj_variants");
            conf_.ss_ljs = lj_variants_.ss;

            auto const& ss_params = pid_params["ss"];
            conf_.ss_lam.psi_0 = ss_params["psi_0"].as<quantity>();
            conf_.ss_lam.alpha = ss_params["alpha"].as<quantity>();

            return conf_;
        });

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        bundles = vm_inst.find_or_emplace<pid_bundle_vector>(
            "pid_bundles").to_span();
        V = &vm_inst.find<real>("V");
    }

    void eval_pid_forces::loop_iter(int idx) const {
        real psi1, psi2;
        vec3r dpsi1_dr1p, dpsi1_dr1, dpsi1_dr1n, dpsi1_dr2;
        vec3r dpsi2_dr2p, dpsi2_dr2, dpsi2_dr2n, dpsi2_dr1;

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

        auto ss_sink_lj = conf.ss_ljs[type];
        if (conf.ss_lam.supp(psi1) && conf.ss_lam.supp(psi2)) {
            auto [lam1, deriv1] = conf.ss_lam(psi1);
            auto [lam2, deriv2] = conf.ss_lam(psi2);
            auto [lj_V, lj_dV_dr] = ss_sink_lj(r12_n, r12_rn);

            V_ += lam1 * lam2 * lj_V;
            A += deriv1 * lam2 * lj_V;
            B += deriv2 * lam1 * lj_V;
            C += lam1 * lam2 * lj_dV_dr;
        }

        auto r12_u = r12 * r12_rn;
        *V += V_;
        F[i1p] -= A * dpsi1_dr1p;
        F[i1] -= A * dpsi1_dr1 + B * dpsi2_dr1 + C * r12_u;
        F[i1n] -= A * dpsi1_dr1n;
        F[i2p] -= B * dpsi2_dr2p;
        F[i2] -= A * dpsi1_dr2 + B * dpsi2_dr2 - C * r12_u;
        F[i2n] -= B * dpsi2_dr2n;
    }

    tf::Task eval_pid_forces::tf_impl(tf::Taskflow &taskflow) const {
        return taskflow.for_each_index(0, std::ref(bundles.size), 1,
            [this](auto idx) -> void { loop_iter(idx); });
    }
}