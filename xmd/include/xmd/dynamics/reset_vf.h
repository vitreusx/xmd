#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class reset_vf: public vm_aware {
    public:
        vec3r_array F;
        real *V;
        int num_particles;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}