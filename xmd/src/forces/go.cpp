#include "forces/go.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {
    void update_go_contacts::operator()() const {
        contacts->clear();

        for (int idx = 0; idx < all_contacts->size(); ++idx) {
            auto nat_cont = all_contacts->at(idx);
            auto idx1 = nat_cont.i1(), idx2 = nat_cont.i2();
            auto nat_dist = nat_cont.nat_dist();

            auto cutoff = lj::cutoff(nat_dist);

            auto r1 = r[idx1], r2 = r[idx2];
            if (norm(box->r_uv(r1, r2)) < cutoff + nl->orig_pad) {
                contacts->push_back(nat_cont);
            }
        }
    }

    void update_go_contacts::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vector<vec3r>>("r").data();
        box = &vm_inst.find<xmd::box>("box");
        nl = &vm_inst.find<nl::nl_data>("nl_data");
        all_contacts = &vm_inst.find_or<vector<nat_cont>>("go_all_contacts",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& all_contacts_ = vm_inst.emplace<vector<nat_cont>>(
                    "go_all_contacts");

                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type != model::NAT_SS) {
                        auto i1 = res_map[cont.res1], i2 = res_map[cont.res2];
                        auto nat_dist = (real)cont.length;
                        all_contacts_.emplace_back(i1, i2, nat_dist);
                    }
                }

                return all_contacts_;
            });

        contacts = &vm_inst.find<vector<nat_cont>>("go_contacts");

        real cutoff = 0.0;
        for (int cont_idx = 0; cont_idx < all_contacts->size(); ++cont_idx) {
            auto cont_dist = all_contacts->at(cont_idx).nat_dist();
            cutoff = max(cutoff, lj::cutoff(cont_dist));
        }

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    void eval_go_forces::operator()() const {
        for (int idx = 0; idx < contacts->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_go_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        depth = vm_inst.find_or_emplace<real>("go_depth",
            params["native contacts"]["lj depth"].as<quantity>());

        box = &vm_inst.find<xmd::box>("box");
        contacts = &vm_inst.find_or_emplace<vector<nat_cont>>(
            "go_contacts");
        r = vm_inst.find<vector<vec3r>>("r").data();
        V = &vm_inst.find<real>("V");
        F = vm_inst.find<vector<vec3r>>("F").data();
    }

    void eval_go_forces::iter(int idx) const {
        auto nat_cont = contacts->at(idx);
        auto i1 = nat_cont.i1(), i2 = nat_cont.i2();
        auto nat_dist = nat_cont.nat_dist();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_rn = norm_inv(r12);

        auto r12_u = r12 * r12_rn;
        auto [V_, dV_dr] = lj(depth, nat_dist)(r12_rn);

        *V += V_;
        F[i1] += r12_u * dV_dr;
        F[i2] -= r12_u * dV_dr;
    }

    void eval_go_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < contacts->size(); ++idx) {
            iter(idx);
        }
    }
}