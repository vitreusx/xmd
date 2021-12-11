#include "forces/es/es_pair.h"
#include <xmd/params/param_file.h>
#include <array>

namespace xmd {
    void update_es_pairs::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            auto q1_q2 = q[(short)atype[idx1]] * q[(short)atype[idx2]];
            if (q1_q2 != 0.0) {
                int pair_idx = pairs->push_back();
                pairs->i1[pair_idx] = idx1;
                pairs->i2[pair_idx] = idx2;
            }
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = cutoff;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }

    void update_es_pairs::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("param_file");

        using stored_q_t = std::array<real, amino_acid::NUM_AA>;
        auto& stored_q = vm_inst.find_or<stored_q_t>("stored_q", [&]() -> auto& {
            auto& stored_q_ = vm_inst.emplace<stored_q_t>("stored_q");

            auto& aa_data_ = vm_inst.find_or_emplace<amino_acid_data>(
                "amino_acid_data");
            for (auto const& aa: amino_acid::all()) {
                stored_q_[(short)aa] = aa_data_[aa].charge;
            }

            return stored_q_;
        });

        for (auto const& aa: amino_acid::all())
            q[(short)aa] = stored_q[(short)aa];

        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl");
        pairs = &vm_inst.find<es_pair_vector>("es_pairs");
        atype = vm_inst.find<vector<amino_acid>>("atype").to_array();
    }

    es_pair_span es_pair_vector::to_span() {
        es_pair_span s;
        s.i1 = i1.data();
        s.i2 = i2.data();
        s.q1_q2 = q1_q2.data();
        s.size = size;
        return s;
    }
}