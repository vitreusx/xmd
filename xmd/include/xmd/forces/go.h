#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/model/box.h>
#include <xmd/utils/math.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/nl/data.h>
#include <xmd/vm/vm.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nat_cont
#define FIELDS() int,i1,int,i2,real,nat_dist

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_go_forces: public vm_aware {
    public:
        real depth;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box<vec3r> const *box;
        vector<nat_cont> const *contacts;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_go_contacts: public vm_aware {
    public:
        const_array<vec3r> r;
        box<vec3r> const *box;
        nl::nl_data const *nl;
        vector<nat_cont> const *all_contacts;
        vector<nat_cont> *contacts;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
