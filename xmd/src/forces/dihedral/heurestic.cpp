#include "forces/dihedral/heurestic.h"
#include <xmd/model/model.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/files/csv.h>
#include <xmd/utils/units.h>

namespace xmd {
    heur_dih_type::heur_dih_type(const amino_acid &a2,
        const amino_acid &a3) {
        auto code2 = (aa_code)a2, code3 = (aa_code)a3;
        int8_t type2 = (code2 == GLY ? (int8_t)0 : (code2 == PRO ? (int8_t)1 : (int8_t)2));
        int8_t type3 = (code3 == GLY ? (int8_t)0 : (code3 == PRO ? (int8_t)1 : (int8_t)2));
        val = (int8_t)3 * type2 + type3;
    }

    constexpr heur_dih_type::operator int8_t() const {
        return val;
    }

    constexpr heur_dih_type::heur_dih_type(int8_t val):
        val{val} {};

    void eval_heurestic_dihedral_forces::operator()() const {
        for (int idx = 0; idx < dihedrals.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_heurestic_dihedral_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");

        using stored_coeffs_t = std::array<std::array<real, NUM_TERMS>, NUM_TYPES>;
        auto& stored_coeffs = ctx.persistent<stored_coeffs_t>("heur_dih_coeffs",
            lazy([&]() -> auto {
                auto coeffs_csv = params["heurestic dihedrals"]["coefficients"].as<csv_file>();
                stored_coeffs_t stored_coeffs_;

                for (auto const& record: coeffs_csv.records) {
                    auto type2 = record["type2"], type3 = record["type3"];
                    auto ord2 = type2 == "G" ? 0 : (type2 == "P" ? 1 : 2);
                    auto ord3 = type3 == "G" ? 0 : (type3 == "P" ? 1 : 2);
                    auto ord = 3*ord2+ord3;

                    stored_coeffs_[0][ord] = quantity(record["sin"], eps);
                    stored_coeffs_[1][ord] = quantity(record["cos"], eps);
                    stored_coeffs_[2][ord] = quantity(record["sin2"], eps);
                    stored_coeffs_[3][ord] = quantity(record["cos2"], eps);
                    stored_coeffs_[4][ord] = quantity(record["sin_cos"], eps);
                }

                return stored_coeffs_;
            }));

        for (int d = 0; d < NUM_TERMS; ++d) {
            for (int t = 0; t < NUM_TYPES; ++t) {
                coeffs[d][t] = stored_coeffs[d][t];
            }
        }

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");

        dihedrals = ctx.persistent<vector<heur_dih>>("heurestic_dihedrals",
            lazy([&]() -> auto {
                auto& xmd_model = ctx.var<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = ctx.var<res_map_t>("res_map");
                auto atypes = ctx.var<vector<amino_acid>>("atype").data();

                vector<heur_dih> dihedrals_;

                for (auto const& dihedral: xmd_model.dihedrals) {
                    if (!dihedral.phi.has_value()) {
                        auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                            i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];
                        auto type = heur_dih_type(atypes[i2], atypes[i3]);

                        dihedrals_.emplace_back(i1, i2, i3, i4, type);
                    }
                }

                return dihedrals_;
            })).view();
    }

    void eval_heurestic_dihedral_forces::iter(int idx) const {
        auto dihedral = dihedrals[idx];
        auto i1 = dihedral.i1(), i2 = dihedral.i2(),
            i3 = dihedral.i3(), i4 = dihedral.i4();
        auto type_val = (int8_t)dihedral.type();

        auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
        auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
        auto x12_23 = cross(r12, r23), x23_34 = cross(r23, r34);

        auto x12_23_rn = norm_inv(x12_23), x23_34_rn = norm_inv(x23_34);
        auto x12_23_u = x12_23 * x12_23_rn, x23_34_u = x23_34 * x23_34_rn;

        auto cos_phi = dot(x12_23_u, x23_34_u);
        auto phi = cos(cos_phi);
        if (dot(x12_23, r34) < 0.0f) phi = -phi;
        auto sin_phi = sin(phi);

        auto sin2_phi = sin_phi*sin_phi, cos2_phi = cos_phi*cos_phi,
            sin_phi_cos_phi = sin_phi*cos_phi;

//#pragma omp atomic update
        *V += coeffs[0][type_val] + coeffs[1][type_val]*sin_phi +
              coeffs[2][type_val]*cos_phi + coeffs[3][type_val]*sin2_phi +
              coeffs[4][type_val]*cos2_phi + coeffs[5][type_val]*sin_phi_cos_phi;

        auto dV_dphi = coeffs[1][type_val]*cos_phi - coeffs[2][type_val]*sin_phi
                       + 2.0f * (coeffs[3][type_val] + coeffs[4][type_val]) * sin_phi_cos_phi
                       + coeffs[5][type_val] * (cos2_phi - sin2_phi);

        auto r23_n = norm(r23);
        auto dphi_dr1 = -x12_23_u * r23_n * x12_23_rn;
        auto dphi_dr4 = x23_34_u * r23_n * x23_34_rn;
        auto df = (-dphi_dr1*dot(r12, r23)+dphi_dr4*dot(r23, r34))/(r23_n*r23_n);
        auto dphi_dr2 = -dphi_dr1 + df;
        auto dphi_dr3 = -dphi_dr4 - df;

        F[i1] -= dV_dphi * dphi_dr1;
        F[i2] -= dV_dphi * dphi_dr2;
        F[i3] -= dV_dphi * dphi_dr3;
        F[i4] -= dV_dphi * dphi_dr4;

    }

    void eval_heurestic_dihedral_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < dihedrals.size(); ++idx) {
            iter(idx);
        }
    }
}