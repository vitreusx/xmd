#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"
#include <xmd/ctx/context.h>

namespace xmd {
    class eval_const_es_forces: public ctx_aware {
    public:
        real permittivity, screen_dist_inv;
        real V_factor;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        vector<es_pair> const *es_pairs;
        real *V;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_const_es: public update_es_base {
    public:
        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}
