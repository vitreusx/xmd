#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>


namespace xmd::qa {
    class precompute_nh {
    public:
        const_array<vec3r> r;
        array<vec3r> n, h;
        box const *box;
        const_array<int> prev, next;
        int num_particles;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}