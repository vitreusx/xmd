#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/ctx/context.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() tether_pair
#define FIELDS() int,i1,int,i2,xmd::real,nat_dist

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_tether_forces: public ctx_aware {
    public:
        real H1, H2;
        real def_length;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        const_span<tether_pair> tethers;
        real *V;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
