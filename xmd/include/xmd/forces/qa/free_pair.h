#pragma once

#include <xmd/nl/data.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>

#define NAMESPACE(...) xmd,qa,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() free_pair
#define FIELDS() int,i1,int,i2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd::qa {
    class update_free_pairs: public vm_aware {
    public:
        real max_formation_min_dist;

    public:
        const_array<vec3r> r;
        box const *box;
        nl::nl_data *nl;
        set<free_pair> *pairs;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}