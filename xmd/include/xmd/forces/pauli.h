#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/utils/math.h>
#include <xmd/nl/data.h>
#include <xmd/forces/primitives/lj.h>


#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() pauli_pair
#define FIELDS() int,i1,int,i2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_pauli_exclusion_forces {
    public:
        real depth, r_excl;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        vector<pauli_pair> const *pairs;
        real *V;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_pauli_pairs {
    public:
        real r_excl;

    public:
        const_array<vec3r> r;
        box const *box;
        nl::nl_data const *nl;
        vector<pauli_pair> *pairs;



    public:
        void operator()() const;
    };
}
