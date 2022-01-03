#include "forces/native_ss.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {
    void update_nat_ssbonds::operator()() const {
        ssbonds->clear();

        for (int idx = 0; idx < all_ssbonds->size(); ++idx) {
            auto ssbond = all_ssbonds->at(idx);
            auto i1 = ssbond.i1(), i2 = ssbond.i2();
            auto r1 = r[i1], r2 = r[i2];
            if (norm(box->r_uv(r1, r2)) < cutoff + nl->orig_pad) {
                ssbonds->emplace_back(i1, i2);
            }
        }
    }

    void update_nat_ssbonds::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vector<vec3r>>("r").data();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl_data");
        all_ssbonds = &vm_inst.find_or<vector<nat_ss>>("all_ssbonds",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& all_ssbonds_ = vm_inst.emplace<vector<nat_ss>>(
                    "all_ssbonds");

                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type == model::NAT_SS) {
                        auto i1 = res_map[cont.res1], i2 = res_map[cont.res2];
                        all_ssbonds_.emplace_back(i1, i2);
                    }
                }

                return all_ssbonds_;
            });

        ssbonds = &vm_inst.find<vector<nat_ss>>("ssbonds");

        auto& nat_r = vm_inst.find<real>("nat_ss_r");
        cutoff = harmonic::cutoff(nat_r);

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_nat_ssbond_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        H1 = vm_inst.find_or_emplace<real>("nat_ss_H1",
            params["native ssbonds"]["H1"].as<quantity>());
        nat_r = vm_inst.find_or_emplace<real>("nat_ss_r",
            params["native ssbonds"]["equilibrium dist"].as<quantity>());

        box = &vm_inst.find<xmd::box<vec3r>>("box");
        ssbonds = &vm_inst.find_or_emplace<vector<nat_ss>>("ssbonds");
        r = vm_inst.find<vector<vec3r>>("r").data();
        V = &vm_inst.find<real>("V");
    }

    void eval_nat_ssbond_forces::iter(int idx) const {
        auto ssbond = ssbonds->at(idx);
        auto cys_i1 = ssbond.i1(), cys_i2 = ssbond.i2();
        auto r1 = r[cys_i1], r2 = r[cys_i2];
        auto r12 = box->r_uv(r1, r2);

        auto r12_n = norm(r12);
        auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

        auto r12_u = r12 / r12_n;
//#pragma omp atomic update
        *V += V_;
        F[cys_i1] += dV_dr * r12_u;
        F[cys_i2] -= dV_dr * r12_u;
    }

    void eval_nat_ssbond_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }
}