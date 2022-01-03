#include "forces/pauli.h"
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>
#include <xmd/forces/primitives/shifted_lj.h>
#include <iostream>

namespace xmd {

    void update_pauli_pairs::operator()() const {
        pairs->clear();

        auto cutoff = r_excl;
        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size(); ++pair_idx) {
            auto pair = nl->particle_pairs[pair_idx];
            auto i1 = pair.i1(), i2 = pair.i2();

            auto r1 = r[i1], r2 = r[i2];
            if (norm_inv(box->r_uv(r1, r2)) > min_norm_inv) {
                pairs->emplace_back(i1, i2);
            }
        }
    }

    void update_pauli_pairs::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vector<vec3r>>("r").data();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl_data");
        pairs = &vm_inst.find<vector<pauli_pair>>("pauli_pairs");

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        r_excl = vm_inst.find<real>("pauli_r_excl");
        max_cutoff = max(max_cutoff, r_excl);
    }

    void eval_pauli_exclusion_forces::operator()() const {
        for (int idx = 0; idx < pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_pauli_exclusion_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        r_excl = vm_inst.find_or_emplace<real>("pauli_r_excl",
            params["Pauli exclusion"]["r_excl"].as<quantity>());
        depth = vm_inst.find_or_emplace<real>("pauli_depth",
            params["Pauli exclusion"]["depth"].as<quantity>());

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        pairs = &vm_inst.find_or_emplace<vector<pauli_pair>>("pauli_pairs");
    }

    void eval_pauli_exclusion_forces::iter(int idx) const {
        auto pair = pairs->at(idx);
        auto i1 = pair.i1(), i2 = pair.i2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_rn = norm_inv(r12);

        if (1.0f < r12_rn * r_excl) {
            auto r12_u = r12 * r12_rn;
            auto [V_, dV_dr] = shifted_lj(depth, r_excl)(r12_rn);

            *V += V_;
            F[i1] += r12_u * dV_dr;
            F[i2] -= r12_u * dV_dr;
        }
    }

    void eval_pauli_exclusion_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < pairs->size(); ++idx) {
            iter(idx);
        }
    }
}