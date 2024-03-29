#pragma once
#include <xmd/types/vec3.h>
#include <xmd/random/rand_gen.h>


namespace xmd {
    class lang_pc_step {
    public:
        real gamma_factor, temperature;
        true_real dt;

    public:
        array<vec3r> r, v, F;
        const_ptr<real> mass, mass_inv, mass_rsqrt;
        real *t;
        array<vec3tr> y0, y1, y2, y3, y4, y5;
        true_real *true_t;
        int num_particles;
        rand_gen *gen;



    public:
        void operator()() const;
    };
}