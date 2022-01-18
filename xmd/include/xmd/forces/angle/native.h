#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>


#define NAMESPACE(...) xmd,__VA_ARGS__
#define NAME() nat_ang
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define FIELDS() int,i1,int,i2,int,i3,xmd::real,nat_theta

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_native_angle_forces {
    public:
        real k;

    public:
        real *V;
        const_array<vec3r> r;
        array<vec3r> F;
        const_span<nat_ang> angles;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
