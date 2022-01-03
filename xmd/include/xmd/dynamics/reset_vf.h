#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class reset_vf: public vm_aware {
    public:
        array<vec3r> F;
        real *V;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
        void omp_async() const;
    };
}