#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <random>
#include <xmd/utils/units.h>
#include <xmd/random/sphere_dist.h>

namespace xmd {
    template<typename Generator>
    class set_initial_vel {
    public:
        real temperature;
        sphere_dist<vec3r, real> vel_dist;

    public:
        Generator& g;
        vec3r_array v;
        array<real> mass_inv;
        int num_particles;

    public:
        explicit set_initial_vel(Generator& g):
            g{g} {};

        void operator()() const {
            auto factor = sqrt(kB * temperature);
            for (int idx = 0; idx < num_particles; ++idx) {
                v[idx] = vec3r::Zero();
            }
        }
    };
}