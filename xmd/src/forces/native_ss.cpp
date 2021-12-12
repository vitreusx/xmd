#include "forces/native_ss.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {
    void update_nat_ssbonds::operator()() const {
        ssbonds->clear();
        
        for (int idx = 0; idx < all_ssobnds->size; ++idx) {
            auto idx1 = all_ssobnds->i1[idx], idx2 = all_ssobnds->i2[idx];
            auto r1 = r[idx1], r2 = r[idx2];
            if (norm(box->ray(r1, r2)) < cutoff + nl->orig_pad) {
                auto cont_idx = ssbonds->push_back();
                ssbonds->i1[cont_idx] = idx1;
                ssbonds->i2[cont_idx] = idx2;
            }
        }
    }

    void update_nat_ssbonds::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl");
        all_ssobnds = &vm_inst.find_or<nat_ssbond_vector>("all_ssbonds",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                int num_ssbonds = 0;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type == model::NAT_SS)
                        ++num_ssbonds;
                }

                auto& all_ssbonds_ = vm_inst.emplace<nat_ssbond_vector>(
                    "all_ssbonds", num_ssbonds);

                int cont_idx = 0;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type == model::NAT_SS) {
                        all_ssbonds_.i1[cont_idx] = res_map[cont.res1];
                        all_ssbonds_.i2[cont_idx] = res_map[cont.res2];
                        ++cont_idx;
                    }
                }

                return all_ssbonds_;
            });
        ssbonds = &vm_inst.find_or_emplace<nat_ssbond_vector>(
            "ssbonds");
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < ssbonds.size; ++idx) {
            auto cys_i1 = ssbonds.i1[idx], cys_i2 = ssbonds.i2[idx];
            auto r1 = r[cys_i1], r2 = r[cys_i2];
            auto r12 = box->ray(r1, r2);

            auto r12_n = norm(r12);
            auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

            auto r12_u = r12 / r12_n;
            *V += V_;
            F[cys_i1] += dV_dr * r12_u;
            F[cys_i2] -= dV_dr * r12_u;
        }
    }

    void eval_nat_ssbond_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        H1 = vm_inst.find_or_emplace<real>("nat_ss_H1",
            params["native ssbonds"]["H1"].as<quantity>());
        nat_r = vm_inst.find_or_emplace<real>("nat_ss_r",
            params["native ssbonds"]["equilibrium dist"].as<quantity>());

        box = &vm_inst.find<xmd::box<vec3r>>("box");
        ssbonds = vm_inst.find<nat_ssbond_vector>("ssbonds").to_span();
        r = vm_inst.find<vec3r_vector>("r").to_array();
        V = &vm_inst.find<real>("V");
    }

    int nat_ssbond_vector::push_back()  {
        i1.push_back();
        i2.push_back();
        return size++;
    }

    void nat_ssbond_vector::clear() {
        i1.clear();
        i2.clear();
        size = 0;
    }

    nat_ssbond_span nat_ssbond_vector::to_span() {
        nat_ssbond_span span;
        span.i1 = i1.data();
        span.i2 = i2.data();
        span.size = size;
        return span;
    }

    nat_ssbond_vector::nat_ssbond_vector(int n):
        i1{n}, i2{n}, size{n} {};
}