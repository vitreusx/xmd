#pragma once
#include <xmd/types/vec3.h>
#include <xmd/random/xorshift64.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class lang_pc_step: public vm_aware {
    public:
        real gamma_factor, temperature;
        true_real dt;

    public:
        vec3r_array r, v, F;
        array<real> mass, mass_inv;
        real *t;
        vec3tr_array y0, y1, y2, y3, y4, y5;
        true_real *true_t;
        int num_particles;
        xorshift64 *gen;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}