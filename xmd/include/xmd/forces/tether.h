#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct tether_pair_span {
        array<int> i1, i2;
        array<real> nat_dist;
        int size;
    };

    class tether_pair_vector {
    public:
        explicit tether_pair_vector(int n = 0);

        tether_pair_span to_span();

        vector<int> i1, i2;
        vector<real> nat_dist;
        int size;
    };

    class eval_tether_forces: public vm_aware {
    public:
        real H1, H2;
        real def_length;

    public:
        vec3r_array r, F;
        tether_pair_span tethers;
        real *V;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
