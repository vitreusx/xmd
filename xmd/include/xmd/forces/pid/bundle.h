#pragma once


#include <xmd/types/amino_acid.h>
#include <xmd/model/box.h>
#include <xmd/types/vec3.h>
#include <xmd/nl/data.h>
#include <xmd/vm/vm.h>

#define NAMESPACE(...) xmd,pid,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() pid_bundle
#define FIELDS() int,i1,int,i2,int16_t,type

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd::pid {
    class update_pid_bundles: public vm_aware {
    public:
        real cutoff;

    public:
        const_array<vec3r> r;
        const_array<int> prev, next;
        const_array<amino_acid> atype;
        box const *box;
        nl::nl_data const *nl;
        vector<pid_bundle> *bundles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}