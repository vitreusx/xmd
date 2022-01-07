#pragma once
#include <xmd/types/vec3.h>
#include <xmd/ctx/context.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() force_afm_tip
#define FIELDS() int,res_idx,xmd::vec3r,pull_force

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_force_afm_forces: public ctx_aware {
    public:
        array<vec3r> F;
        span<force_afm_tip> afm_tips;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}