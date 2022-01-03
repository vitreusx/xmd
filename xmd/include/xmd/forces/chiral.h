#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/math.h>
#include <xmd/vm/vm.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() chiral_quad
#define FIELDS() int,i1,int,i2,int,i3,int,i4,real,nat_chir,real,nat_factor

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_chiral_forces: public vm_aware {
    public:
        real e_chi;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        span<chiral_quad> quads;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
