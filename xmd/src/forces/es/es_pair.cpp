#include "forces/es/es_pair.h"

namespace xmd {

    void update_es_pairs::operator()() const {
        pairs->clear();

        auto f = [&](auto idx1, auto idx2) -> auto {
            int pair_idx = pairs->push_back();
            pairs->i1[pair_idx] = idx1;
            pairs->i2[pair_idx] = idx2;
        };

        nl::iter_over_pairs iter(f);
        iter.cutoff = cutoff;
        iter.box = box;
        iter.nl = nl;
        iter.r = r;

        iter();
    }

    void update_es_pairs::bind_to_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl");
        pairs = &vm_inst.find<es_pair_vector>("es_pairs");
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