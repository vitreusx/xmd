#include "forces/qa/process_contacts.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd::qa {

    void process_contacts::operator()() const {
        real factor = breaking_factor * (real)pow(2.0f, -1.0f/6.0f);

//#pragma omp taskloop default(none) private(factor) nogroup
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            if (!contacts->has_item(idx))
                continue;

            auto i1 = contacts->i1[idx], i2 = contacts->i2[idx];
            auto type = contacts->type[idx];
            auto status = contacts->status[idx];
            auto ref_time = contacts->ref_time[idx];
            auto sync_diff1 = contacts->sync_diff1[idx];
            auto sync_diff2 = contacts->sync_diff2[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);
            auto r12_u = r12 * r12_rn;

            auto saturation = min(*t - ref_time, cycle_time) * cycle_time_inv;
            if (status == BREAKING)
                saturation = 1.0f - saturation;

            auto lj_force = ljs[(short)type];
            auto r12_n = r12_rn * v3::norm_squared(r12);
            auto [Vij, dVij_dr] = lj_force(r12_n, r12_rn);
//#pragma omp atomic update
            *V += saturation * Vij;
            auto f = saturation * dVij_dr * r12_u;
            F[i1] += f;
            F[i2] -= f;

            if (status == FORMING_OR_FORMED && saturation == 1.0f) {
                if (factor * lj_force.r_min * r12_rn < 1.0f) {
                    contacts->status[idx] = BREAKING;
                    contacts->ref_time[idx] = *t;
                }
            }
            else if (status == BREAKING && saturation == 0.0f) {
#pragma omp critical
                contacts->remove(idx);

                sync[i1] += sync_diff1;
                sync[i2] += sync_diff2;

                int free_idx;
#pragma omp critical
                { free_idx = free_pairs->add(); };

                free_pairs->i1[free_idx] = i1;
                free_pairs->i2[free_idx] = i2;
            }
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

        sync = vm_inst.find<sync_data_vector>("sync").to_array();
        contacts = &vm_inst.find<contact_set>("qa_contacts");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        V = &vm_inst.find<real>("V");
        F = vm_inst.find<vec3r_vector>("F").to_array();
        r = vm_inst.find<vec3r_vector>("r").to_array();
        free_pairs = &vm_inst.find<free_pair_set>("qa_free_pairs");
    }
}