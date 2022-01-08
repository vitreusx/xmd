#pragma once
#include <xmd/types/vec3.h>
#include <xmd/ctx/context.h>

namespace xmd {
    class reduce_vf: public ctx_aware {
    public:
        array<vec3r> F, thread_F;
        real *V, *thread_V;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}