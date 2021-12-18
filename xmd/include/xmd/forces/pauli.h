#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/utils/math.h>
#include <xmd/nl/data.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct pauli_pair_span {
        array<int> i1, i2;
        int size;
    };

    struct pauli_pair_vector {
        vector<int> i1, i2;
        int size;

        explicit pauli_pair_vector(int n = 0);

        int push_back();
        void clear();
        pauli_pair_span to_span();
    };

    class eval_pauli_exclusion_forces: public vm_aware {
    public:
        real depth, r_excl;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        pauli_pair_span pairs;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };

    class update_pauli_pairs: public vm_aware {
    public:
        real r_excl;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl::nl_data *nl;
        pauli_pair_vector *pairs;
        eval_pauli_exclusion_forces *eval;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
