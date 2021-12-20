#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class reduce_vf_omp: public vm_aware {
    public:
        vec3r_array shared_F, thread_F;
        real *shared_V, *thread_V;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}