#include "forces/velocity_afm.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_velocity_afm_forces::operator()() const {
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_velocity_afm_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        afm_force.H1 = ctx.persistent<real>("vel_afm_H1",
            params["velocity AFM"]["H1"].as<quantity>());
        afm_force.H2 = ctx.persistent<real>("vel_afm_H2",
            params["velocity AFM"]["H2"].as<quantity>());

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.per_thread().var<vector<vec3r>>("F").data();
        t = &ctx.var<real>("t");

        afm_tips = ctx.persistent<vector<vel_afm_tip>>("vel_afm_tips",
            lazy([&]() -> auto {
                vector<vel_afm_tip> afm_tips_;

                for (auto const& tip_node: params["velocity AFM"]["AFM tips"]) {
                    auto res_idx = tip_node["residue idx"].as<int>();
                    auto afm_orig = tip_node["origin"].as<vec3r>();
                    auto afm_vel = tip_node["velocity"].as<vec3r>();

                    afm_tips_.emplace_back(res_idx, afm_orig, afm_vel);
                }

                return afm_tips_;
            })).view();
    }

    void eval_velocity_afm_forces::iter(int idx) const {
        auto tip = afm_tips[idx];
        auto r_ = r[tip.res_idx()];
        auto cur_afm_pos = tip.afm_orig() + *t * tip.afm_vel();

        auto r_afm = cur_afm_pos - r_;
        auto r_afm_n = norm(r_afm), r_afm_rn = 1.0f / r_afm_n;
        auto r_afm_u = r_afm * r_afm_rn;

        auto [_, dV_dr] = afm_force(r_afm_n);
        F[tip.res_idx()] += dV_dr * r_afm_u;
    }

    void eval_velocity_afm_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }
}