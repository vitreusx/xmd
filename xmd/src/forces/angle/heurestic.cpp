#include "forces/angle/heurestic.h"
#include <xmd/model/model.h>
#include <xmd/params/param_file.h>
#include <xmd/files/csv.h>
#include <xmd/utils/units.h>
#include <sstream>

namespace xmd {
    heur_ang_type::heur_ang_type(const amino_acid &a1,
        const amino_acid &a2) {
        auto code1 = (aa_code)a1, code2 = (aa_code)a2;
        int8_t type1 = (code1 == GLY ? (int8_t)0 : (code1 == PRO ? (int8_t)1 : (int8_t)2));
        int8_t type2 = (code2 == GLY ? (int8_t)0 : (code2 == PRO ? (int8_t)1 : (int8_t)2));
        val = (int8_t)3 * type1 + type2;
    }

    constexpr heur_ang_type::operator int8_t() const {
        return val;
    }

    constexpr heur_ang_type::heur_ang_type(int8_t val) :
        val{val} {};

    void eval_heurestic_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_heurestic_angle_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");

        using stored_coeffs_t = std::array<std::array<real, POLY_DEG+1>, NUM_TYPES>;
        auto& stored_poly_coeffs = vm_inst.find_or<stored_coeffs_t>("heur_ang_coeffs",
            [&]() -> auto& {
                auto coeffs_csv = params["heurestic angles"]["coefficients"].as<csv_file>();
                auto& stored_poly_coeffs_ = vm_inst.emplace<stored_coeffs_t>(
                    "heur_ang_coeffs");

                for (auto const& record: coeffs_csv.records) {
                    auto type1 = record["type1"], type2 = record["type2"];
                    auto ord1 = type1 == "G" ? 0 : (type1 == "P" ? 1 : 2);
                    auto ord2 = type2 == "G" ? 0 : (type2 == "P" ? 1 : 2);
                    auto ord = 3*ord1+ord2;

                    for (int d = 0; d <= POLY_DEG; ++d) {
                        std::stringstream sstr;
                        sstr << "a" << d;
                        auto col_name = sstr.str();

                        stored_poly_coeffs_[d][ord] = quantity(
                            record[col_name], eps/pow(rad, d));
                    }
                }

                return stored_poly_coeffs_;
            });

        for (int d = 0; d <= POLY_DEG; ++d) {
            for (int t = 0; t < NUM_TYPES; ++t) {
                poly_coeffs[d][t] = stored_poly_coeffs[d][t];
            }
        }

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");

        angles = vm_inst.find_or<vector<heur_ang>>("heurestic_angles",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");
                auto atypes = vm_inst.find<vector<amino_acid>>("atype").data();

                auto& angles_ = vm_inst.emplace<vector<heur_ang>>(
                    "heurestic_angles");

                for (auto const& angle: xmd_model.angles) {
                    if (!angle.theta.has_value()) {
                        auto i1 = res_map[angle.res1], i2 = res_map[angle.res2],
                            i3 = res_map[angle.res3];
                        auto type = heur_ang_type(atypes[i1], atypes[i2]);

                        angles_.emplace_back(i1, i2, i3, type);
                    }
                }

                return angles_;
            }).view();
    }

    void eval_heurestic_angle_forces::iter(int idx) const {
        auto angle = angles[idx];
        auto i1 = angle.i1(), i2 = angle.i2(), i3 = angle.i3();
        auto type_val = (int8_t)angle.type();

        auto r1 = r[i1], r2 = r[i2], r3 = r[i3];
        auto r12 = r2 - r1, r23 = r3 - r2;

        auto x12_23 = cross(r12, r23);
        auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);

        auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
        auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
        auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

        auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
        auto theta = acos(cos_theta);

        real angle_V = 0.0f, dV_dtheta = 0.0f;
        for (int d = POLY_DEG; d >= 0; --d) {
            auto coeff = poly_coeffs[d][type_val];
            if (d > 0) dV_dtheta = (real)d * coeff + theta * dV_dtheta;
            angle_V = coeff + theta * angle_V;
        }

        *V += angle_V;

        F[i1] -= dV_dtheta * dtheta_dr1;
        F[i2] -= dV_dtheta * dtheta_dr2;
        F[i3] -= dV_dtheta * dtheta_dr3;
    }

    void eval_heurestic_angle_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }
}