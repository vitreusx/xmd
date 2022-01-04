#include "forces/es/const.h"
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {

    void eval_const_es_forces::operator()() const {
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_const_es_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        auto const& const_es_params = params["const ES"];

        permittivity = vm_inst.find_or_add<real>("permittivity",
            const_es_params["permittivity"].as<quantity>());
        V_factor = 1.0f / (4.0f * (real)M_PI * permittivity);

        auto screening_dist = vm_inst.find_or_add<real>("screening_dist",
            const_es_params["screening distance"].as<quantity>());
        screen_dist_inv = (real)1.0 / screening_dist;

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box>("box");
        es_pairs = &vm_inst.find_or_emplace<vector<es_pair>>("es_pairs");
    }

    void eval_const_es_forces::iter(int idx) const {
        auto es = es_pairs->at(idx);
        auto i1 = es.i1(), i2 = es.i2();
        auto q1_x_q2 = es.q1_x_q2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
        auto r12_u = r12 * r12_rn;

        auto Vij = V_factor * q1_x_q2 * exp(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
        auto dVij_dr = -Vij*(screen_dist_inv+r12_rn);

        *V += Vij;

        auto f = r12_u * dVij_dr;
        F[i1] += f;
        F[i2] -= f;

    }

    void eval_const_es_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void update_const_es::init_from_vm(vm &vm_inst) {
        update_es_base::init_from_vm(vm_inst);

        auto& params = vm_inst.find<param_file>("params");
        auto const& const_es_params = params["const ES"];
        auto screening_dist = vm_inst.find_or_emplace<real>("screening_dist",
            const_es_params["screening distance"].as<quantity>());
        cutoff = 2.0 * screening_dist;

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    void update_const_es::operator()() const {
        update_es_base::operator()();
    }
}