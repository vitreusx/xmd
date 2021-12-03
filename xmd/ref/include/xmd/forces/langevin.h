#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/units.h>
#include <xmd/random/xorshift64.h>

namespace xmd {
    class eval_langevin_dynamics {
    public:
        float gamma_factor, temperature;

    public:
        vec3f_array F, v;
        array<float> mass;
        int num_particles;
        xorshift64 *gen;

    public:
        void operator()() const;
    };
}