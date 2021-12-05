#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/utils/math.h>
#include <xmd/nl/nl_data.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    struct pauli_pair_span {
        array<int> i1, i2;
        int size;
    };

    struct pauli_pair_vector {
        vector<int> i1, i2;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            size = 0;
        }

        inline auto to_span() const {
            pauli_pair_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.size = size;
            return s;
        }
    };

    class update_pauli_pairs {
    public:
        real r_excl;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl::nl_data *nl;
        pauli_pair_vector *pairs;

    public:
        void operator()() const;
    };

    class eval_pauli_exclusion_forces {
    public:
        real depth, r_excl;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        pauli_pair_span pairs;
        real *V;

    public:
        void operator()() const;
    };
}
