#pragma once
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/model/box.h>


#include <xmd/nl/data.h>
#include <xmd/types/vec3.h>


#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nat_ss
#define FIELDS() int,i1,int,i2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_nat_ssbond_forces {
    public:
        real H1, nat_r;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        vector<nat_ss> const *ssbonds;
        real *V;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_nat_ssbonds {
    public:
        real cutoff;

    public:
        array<vec3r> r;
        box const *box;
        nl::nl_data const *nl;
        vector<nat_ss> const *all_ssbonds;
        vector<nat_ss> *ssbonds;



    public:
        void operator()() const;
    };
}