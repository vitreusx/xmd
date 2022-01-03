#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class setup_vf_omp: public vm_aware {
    public:
        array<vec3r> thread_F;
        real *thread_V;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}