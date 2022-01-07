#pragma once
#include <xmd/types/vec3.h>
#include <xmd/ctx/context.h>

namespace xmd {
    class reset_vf: public ctx_aware {
    public:
        array<vec3r> F;
        real *V;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
        void omp_async() const;
    };
}