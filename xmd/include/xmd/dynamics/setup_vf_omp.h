#pragma once
#include <xmd/types/vec3.h>
#include <xmd/ctx/context.h>

namespace xmd {
    class setup_vf_omp: public ctx_aware {
    public:
        array<vec3r> thread_F;
        real *thread_V;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}