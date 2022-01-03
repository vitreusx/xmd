#include "forces/qa/process_contacts.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd::qa {

    void process_contacts::operator()() const {
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            iter(idx);
        }
    }

    void process_contacts::init_from_vm(vm &vm_inst) {
        ljs = vm_inst.find<lj_variants>("lj_variants");

        auto& params = vm_inst.find<param_file>("params");
        auto const& contact_params = params["quasi-adiabatic"];
        cycle_time = vm_inst.find_or_emplace<real>("qa_cycle_time",
            contact_params["(de)saturation time"].as<quantity>());
        cycle_time_inv = (real)1.0/cycle_time;
        breaking_factor = vm_inst.find_or_emplace<real>("qa_breaking_factor",
            contact_params["breaking factor"].as<quantity>());
        t = &vm_inst.find<real>("t");

        factor = breaking_factor * (real)pow(2.0f, -1.0f/6.0f);

        sync = vm_inst.find<vector<sync_data>>("sync").data();
        contacts = &vm_inst.find<set<contact>>("qa_contacts");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        V = &vm_inst.find<real>("V");
        F = vm_inst.find<vector<vec3r>>("F").data();
        r = vm_inst.find<vector<vec3r>>("r").data();
        free_pairs = &vm_inst.find<set<free_pair>>("qa_free_pairs");
    }

    void process_contacts::iter(int idx) const {
        auto node = contacts->at(idx);
        if (node.vacant()) return;
        auto contact = node.value();

        auto i1 = contact.i1(), i2 = contact.i2();
        auto type = contact.type();
        auto status = contact.status();
        auto ref_time = contact.ref_time();
        auto sync_diff1 = contact.sync_diff1();
        auto sync_diff2 = contact.sync_diff2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_rn = norm_inv(r12);
        auto r12_u = r12 * r12_rn;

        auto saturation = min(*t - ref_time, cycle_time) * cycle_time_inv;
        if (status == BREAKING)
            saturation = 1.0f - saturation;

        auto lj_force = ljs[(short)type];
        auto r12_n = r12_rn * norm_squared(r12);
        auto [Vij, dVij_dr] = lj_force(r12_n, r12_rn);
        *V += saturation * Vij;
        auto f = saturation * dVij_dr * r12_u;
        F[i1] += f;
        F[i2] -= f;

        if (status == FORMING_OR_FORMED && saturation == 1.0f) {
            if (factor * lj_force.r_min() * r12_rn < 1.0f) {
                contact.status() = BREAKING;
                contact.ref_time() = *t;
            }
        }
        else if (status == BREAKING && saturation == 0.0f) {
#pragma omp critical
            {
                contacts->remove(idx);
                sync[i1] += sync_diff1;
                sync[i2] += sync_diff2;
                free_pairs->emplace(i1, i2);
            }
        }
    }

    void process_contacts::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            iter(idx);
        }
    }
}