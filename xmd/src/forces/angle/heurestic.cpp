#include "forces/angle/heurestic.h"
#include <xmd/model/model.h>
#include <xmd/params/param_file.h>
#include <xmd/files/csv.h>
#include <xmd/utils/units.h>
#include <sstream>

namespace xmd {

    heurestic_angle_type::heurestic_angle_type(const amino_acid &a1,
        const amino_acid &a2) {
        auto code1 = (aa_code)a1, code2 = (aa_code)a2;
        int8_t type1 = (code1 == GLY ? (int8_t)0 : (code1 == PRO ? (int8_t)1 : (int8_t)2));
        int8_t type2 = (code2 == GLY ? (int8_t)0 : (code2 == PRO ? (int8_t)1 : (int8_t)2));
        val = (int8_t)3 * type1 + type2;
    }

    constexpr heurestic_angle_type::operator int8_t() {
        return val;
    }

    constexpr heurestic_angle_type::heurestic_angle_type(int8_t val) :
        val{val} {};

    void eval_heurestic_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size; ++idx)
            loop_iter(idx);
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

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");

        angles = vm_inst.find_or<heurestic_angle_vector>("heurestic_angles",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                int num_heur_ang = 0;
                for (auto const& angle: xmd_model.angles) {
                    if (!angle.theta.has_value())
                        num_heur_ang += 1;
                }

                auto& angles_vec = vm_inst.emplace<heurestic_angle_vector>(
                    "heurestic_angles", num_heur_ang);

                int ang_idx = 0;
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");
                auto atypes = vm_inst.find<vector<amino_acid>>("atype").to_array();
                for (auto const& angle: xmd_model.angles) {
                    if (!angle.theta.has_value()) {
                        auto i1 = res_map[angle.res1], i2 = res_map[angle.res2],
                            i3 = res_map[angle.res3];

                        angles_vec.i1[ang_idx] = i1;
                        angles_vec.i2[ang_idx] = i2;
                        angles_vec.i3[ang_idx] = i3;
                        auto type = heurestic_angle_type(atypes[i1], atypes[i2]);
                        angles_vec.type[ang_idx] = type;

                        ++ang_idx;
                    }
                }

                return angles_vec;
            }).to_span();
    }

    void eval_heurestic_angle_forces::loop_iter(int idx) const {
        auto i1 = angles.i1[idx], i2 = angles.i2[idx], i3 = angles.i3[idx];
        auto type_val = (int8_t)angles.type[idx];

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

    tf::Task eval_heurestic_angle_forces::tf_impl(tf::Taskflow& taskflow) const {
        return taskflow.for_each_index(0, angles.size, 1,
            [this](auto idx) -> void { loop_iter(idx); });
    }

    heurestic_angle_span heurestic_angle_vector::to_span()  {
        heurestic_angle_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.type = type.to_array();
        span.size = i1.size();
        return span;
    }

    heurestic_angle_vector::heurestic_angle_vector(int n):
        i1{n}, i2{n}, i3{n}, type{n}, size{n} {};
}