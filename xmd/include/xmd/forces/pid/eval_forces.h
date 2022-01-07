#pragma once
#include "lambda.h"
#include "bundle.h"
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>
#include <xmd/ctx/context.h>
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::pid {
    class eval_pid_forces: public ctx_aware {
    public:
        struct conf_t {
            lambda_func bb_plus_lam, bb_minus_lam, ss_lam;
            lj bb_plus_lj, bb_minus_lj;
            const_array<sink_lj> ss_ljs;
        };
        conf_t conf;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        vector<pid_bundle> const *bundles;
        real *V;
        array<int> prev, next;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()();
        void omp_async() const;
    };
}