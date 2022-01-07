#include "forces/qa/process_contacts.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd::qa {

    void process_contacts::operator()() const {
        for (int idx = 0; idx < contacts->extent(); ++idx) {
            iter(idx);
        }
    }

    void process_contacts::declare_vars(context& ctx) {
        ljs = ctx.var<lj_variants>("lj_variants");

        auto& params = ctx.var<yaml_fs_node>("params");
        auto const& contact_params = params["quasi-adiabatic"];
        cycle_time = ctx.persistent<real>("qa_cycle_time",
            contact_params["(de)saturation time"].as<quantity>());
        cycle_time_inv = (real)1.0/cycle_time;
        breaking_factor = ctx.persistent<real>("qa_breaking_factor",
            contact_params["breaking factor"].as<quantity>());
        t = &ctx.var<real>("t");

        factor = breaking_factor * (real)pow(2.0f, -1.0f/6.0f);

        sync = ctx.var<vector<sync_data>>("sync").data();
        contacts = &ctx.var<set<contact>>("qa_contacts");
        box = &ctx.var<xmd::box>("box");
        V = &ctx.var<real>("V");
        F = ctx.var<vector<vec3r>>("F").data();
        r = ctx.var<vector<vec3r>>("r").data();
        free_pairs = &ctx.var<set<free_pair>>("qa_free_pairs");
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