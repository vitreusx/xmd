#pragma once
#include <xmd/vm/vm.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_asphericity: public vm_aware {
    public:
        real *asphericity;
        vec3r_array r;
        array<real> mass;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}