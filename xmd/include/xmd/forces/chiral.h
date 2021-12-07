#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/math.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct chiral_quad_span {
        array<int> i1, i2, i3, i4;
        array<real> nat_chir, nat_factor;
        int size;
    };

    class chiral_quad_vector {
    public:
        vector<int> i1, i2, i3, i4;
        vector<real> nat_chir, nat_factor;
        int size;

        explicit chiral_quad_vector(int n = 0);

        chiral_quad_span to_span();
    };

    class eval_chiral_forces: public vm_aware {
    public:
        real e_chi;

    public:
        vec3r_array r, F;
        chiral_quad_span quads;
        real *V;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
