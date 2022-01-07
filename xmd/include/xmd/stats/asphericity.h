#pragma once
#include <xmd/ctx/context.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_asphericity: public ctx_aware {
    public:
        real *asphericity;
        const_array<vec3r> r;
        array<real> mass;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}