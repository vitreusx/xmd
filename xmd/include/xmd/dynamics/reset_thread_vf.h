#pragma once
#include <xmd/ctx/context.h>
#include <xmd/types/vec3.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class reset_thread_vf: public ctx_aware {
    public:
        span<vec3r> thread_F;
        real *thread_V;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}