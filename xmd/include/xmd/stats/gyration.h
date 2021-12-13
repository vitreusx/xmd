#pragma once
#include <xmd/vm/vm.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_gyration_radius: public vm_aware {
    public:
        real* gyration_radius;
        vec3r_array r;
        array<real> mass;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}