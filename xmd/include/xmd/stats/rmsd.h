#pragma once
#include <xmd/ctx/context.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class compute_rmsd: public ctx_aware {
    public:
        real *rmsd;
        const_array<vec3r> r;
        array<vec3r> ref_r;
        int num_particles;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}