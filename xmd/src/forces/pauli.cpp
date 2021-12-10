#include "forces/pauli.h"
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {

    void update_pauli_pairs::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            int pair_idx = pairs->push_back();
            pairs->i1[pair_idx] = idx1;
            pairs->i2[pair_idx] = idx2;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = r_excl;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }

    void update_pauli_pairs::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        r_excl = vm_inst.find_or_emplace<real>("pauli_r_excl",
            params["Pauli exclusion"]["r_excl"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl");
        pairs = &vm_inst.find<pauli_pair_vector>("pauli_pairs");
    }

    void eval_pauli_exclusion_forces::operator()() const {
        for (int idx = 0; idx < pairs.size; ++idx) {
            auto i1 = pairs.i1[idx], i2 = pairs.i2[idx];
            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);

            auto within = (1.0f < r12_rn * r_excl);
            if (within) {
                auto r12_u = r12 * r12_rn;
                auto [V_, dV_dr] = lj(depth, r_excl)(r12_rn);

                *V += V_;
                F[i1] += r12_u * dV_dr;
                F[i2] -= r12_u * dV_dr;
            }
        }
    }

    void eval_pauli_exclusion_forces::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        pairs = vm_inst.find_or_emplace<pauli_pair_vector>(
            "pauli_pairs").to_span();
    }

    int pauli_pair_vector::push_back()  {
        i1.push_back();
        i2.push_back();
        return size++;
    }

    pauli_pair_vector::pauli_pair_vector(int n):
        i1{n}, i2{n}, size{n} {};

    void pauli_pair_vector::clear() {
        i1.clear();
        i2.clear();
        size = 0;
    }

    pauli_pair_span pauli_pair_vector::to_span() {
        pauli_pair_span s;
        s.i1 = i1.data();
        s.i2 = i2.data();
        s.size = size;
        return s;
    }
}