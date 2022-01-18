#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>


#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() vel_afm_tip
#define FIELDS() int,res_idx,xmd::vec3r,afm_orig,xmd::vec3r,afm_vel

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_velocity_afm_forces {
    public:
        harmonic afm_force;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        real *t;
        const_span<vel_afm_tip> afm_tips;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}