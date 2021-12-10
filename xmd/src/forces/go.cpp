#include "forces/go.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {
    void update_go_contacts::operator()() const {
        contacts->clear();
        for (int idx = 0; idx < all_contacts->size; ++idx) {
            auto idx1 = all_contacts->i1[idx], idx2 = all_contacts->i2[idx];
            auto nat_dist = all_contacts->nat_dist[idx];

            auto r1 = r[idx1], r2 = r[idx2];
            if (norm(box->ray(r1, r2)) < nat_dist + nl->pad) {
                auto cont_idx = contacts->push_back();
                contacts->i1[cont_idx] = idx1;
                contacts->i2[cont_idx] = idx2;
                contacts->nat_dist[cont_idx] = nat_dist;
            }
        }
    }

    void update_go_contacts::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl");
        all_contacts = &vm_inst.find_or<go_contact_vector>("go_all_contacts",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                int num_go_cont = 0;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type != model::NAT_SS)
                        ++num_go_cont;
                }

                auto& all_contacts_ = vm_inst.emplace<go_contact_vector>(
                    "go_all_contacts", num_go_cont);

                int cont_idx = 0;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type != model::NAT_SS) {
                        all_contacts_.i1[cont_idx] = res_map[cont.res1];
                        all_contacts_.i2[cont_idx] = res_map[cont.res2];
                        all_contacts_.nat_dist[cont_idx] = (real)cont.length;
                        ++cont_idx;
                    }
                }

                return all_contacts_;
            });
        contacts = &vm_inst.find_or_emplace<go_contact_vector>(
            "go_contacts");
    }

    void eval_go_forces::operator()() const {
        for (int idx = 0; idx < contacts.size; ++idx) {
            auto i1 = contacts.i1[idx], i2 = contacts.i2[idx];
            auto nat_dist = contacts.nat_dist[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);

            auto r12_u = r12 * r12_rn;
            auto [V_, dV_dr] = lj(depth, nat_dist)(r12_rn);

            *V += V_;
            F[i1] += r12_u * dV_dr;
            F[i2] -= r12_u * dV_dr;
        }
    }

    void eval_go_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        depth = vm_inst.find_or_emplace<real>("go_depth",
            params["native contacts"]["lj depth"].as<quantity>());

        box = &vm_inst.find<xmd::box<vec3r>>("box");
        contacts = vm_inst.find<go_contact_vector>("go_contacts").to_span();
        r = vm_inst.find<vec3r_vector>("r").to_array();
        V = &vm_inst.find<real>("V");
    }

    go_contact_vector::go_contact_vector(int n):
        i1{n}, i2{n}, nat_dist{n} {};
}