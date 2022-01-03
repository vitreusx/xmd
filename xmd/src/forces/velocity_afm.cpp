#include "forces/velocity_afm.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_velocity_afm_forces::operator()() const {
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_velocity_afm_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        afm_force.H1 = vm_inst.find_or_emplace<real>("vel_afm_H1",
            params["velocity AFM"]["H1"].as<quantity>());
        afm_force.H2 = vm_inst.find_or_emplace<real>("vel_afm_H2",
            params["velocity AFM"]["H2"].as<quantity>());

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        t = &vm_inst.find<real>("t");

        afm_tips = vm_inst.find_or<vector<vel_afm_tip>>("vel_afm_tips",
            [&]() -> auto& {
                auto& afm_tips_ = vm_inst.emplace<vector<vel_afm_tip>>(
                    "vel_afm_bundles");

                for (auto const& tip_node: params["velocity AFM"]["AFM tips"]) {
                    auto res_idx = tip_node["residue idx"].as<int>();
                    auto afm_orig = tip_node["origin"].as<vec3r>();
                    auto afm_vel = tip_node["velocity"].as<vec3r>();

                    afm_tips_.emplace_back(res_idx, afm_orig, afm_vel);
                }

                return afm_tips_;
            }).view();
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
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }
}