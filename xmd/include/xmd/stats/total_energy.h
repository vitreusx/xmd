#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class compute_total_energy: public vm_aware {
    public:
        vec3r_array v;
        array<real> mass;
        int num_particles;
        real *K, *V, *E;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}