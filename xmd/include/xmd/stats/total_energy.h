#pragma once
#include <xmd/types/vec3.h>
#include <xmd/ctx/context.h>

namespace xmd {
    class compute_total_energy: public ctx_aware {
    public:
        const_array<vec3r> v;
        array<real> mass;
        int num_particles;
        real *K, *V, *E;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}