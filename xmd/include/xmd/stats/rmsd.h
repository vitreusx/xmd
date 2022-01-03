#pragma once
#include <xmd/vm/vm.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_rmsd: public vm_aware {
    public:
        real *rmsd;
        const_array<vec3r> r;
        array<vec3r> ref_r;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}