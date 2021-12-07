#pragma once
#include <xmd/types/vec3.h>
#include <xmd/random/xorshift64.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class lang_leapfrog_step: public vm_aware {
    public:
        real gamma_factor, temperature;
        true_real dt;

    public:
        vec3r_array r, F;
        vec3tr_array true_r, true_prev_v, prev_a;
        real *t;
        true_real *true_t;
        int num_particles;
        array<real> mass, mass_inv;
        xorshift64 *gen;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}