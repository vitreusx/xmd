#pragma once
#include <xmd/ctx/context.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_gyration_radius: public ctx_aware {
    public:
        real* gyration_radius;
        const_array<vec3r> r;
        array<real> mass;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}