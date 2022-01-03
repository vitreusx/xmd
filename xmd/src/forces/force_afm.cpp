#include "forces/force_afm.h"
#include <xmd/params/param_file.h>

namespace xmd {
    void eval_force_afm_forces::operator()() const {
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_force_afm_forces::init_from_vm(vm &vm_inst) {
        F = vm_inst.find<vector<vec3r>>("F").data();

        afm_tips = vm_inst.find_or<vector<force_afm_tip>>("force_afm_tips",
            [&]() -> auto& {
                auto& afm_tips_ = vm_inst.emplace<vector<force_afm_tip>>(
                    "force_afm_tips");

                auto& params = vm_inst.find<param_file>("params");
                for (auto const& tip_node: params["force AFM"]["AFM tips"]) {
                    auto res_idx = tip_node["residue idx"].as<int>();
                    auto pull_force = tip_node["pull force"].as<vec3r>();
                    afm_tips_.emplace_back(res_idx, pull_force);
                }

                return afm_tips_;
            }).view();
    }

    void eval_force_afm_forces::iter(int idx) const {
        auto tip = afm_tips[idx];
        F[tip.res_idx()] += tip.pull_force();
    }

    void eval_force_afm_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < afm_tips.size(); ++idx) {
            iter(idx);
        }
    }
}