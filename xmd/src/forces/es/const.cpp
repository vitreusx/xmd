#include "forces/es/const.h"
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {

    void eval_const_es_forces::operator()() const {
        for (int idx = 0; idx < es_pairs.size; ++idx) {
            iter(idx);
        }
    }

    void eval_const_es_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        auto const& es_params = params["electrostatic forces"];
        auto const& const_es_params = es_params["const variant params"];

        permittivity = vm_inst.find_or_add<real>("permittivity",
            const_es_params["permittivity"].as<quantity>());
        V_factor = 1.0f / (4.0f * (real)M_PI * permittivity);

        auto screening_dist = vm_inst.find_or_add<real>("screening_dist",
            es_params["screening distance"].as<quantity>());
        screen_dist_inv = (real)1.0 / screening_dist;

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        es_pairs = vm_inst.find_or_emplace<es_pair_vector>("es_pairs").to_span();
    }

    void eval_const_es_forces::iter(int idx) const {
        auto i1 = es_pairs.i1[idx], i2 = es_pairs.i2[idx];
        auto q1_q2 = es_pairs.q1_q2[idx];

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->ray(r1, r2);
        auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
        auto r12_u = r12 * r12_rn;

        auto Vij = V_factor * q1_q2 * exp(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
        auto dVij_dr = -Vij*(screen_dist_inv+r12_rn);

//#pragma omp atomic update
        *V += Vij;

        auto f = r12_u * dVij_dr;
        F[i1] += f;
        F[i2] -= f;

    }

    void eval_const_es_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < es_pairs.size; ++idx) {
            iter(idx);
        }
    }

    void update_const_es::init_from_vm(vm &vm_inst) {
        update_es_base::init_from_vm(vm_inst);
        eval = &vm_inst.find<eval_const_es_forces>("eval_const_es");
    }

    void update_const_es::operator()() const {
        update_es_base::operator()();
        eval->es_pairs = pairs->to_span();
    }
}