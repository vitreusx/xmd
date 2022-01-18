#pragma once


#include <xmd/nl/data.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

#include <xmd/types/amino_acid.h>
#include <array>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() es_pair
#define FIELDS() int,i1,int,i2,xmd::real,q1_x_q2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class update_es_base {
    public:
        real cutoff;
        real q[amino_acid::NUM_AA];

    public:
        const_array<vec3r> r;
        box const* box;
        nl::nl_data const* nl;
        vector<es_pair> *pairs;
        const_array<amino_acid> atype;



    public:
        void operator()() const;
    };
}