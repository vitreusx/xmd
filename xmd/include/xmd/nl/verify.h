#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/ctx/context.h>
#include "data.h"

namespace xmd::nl {
    class verify: public ctx_aware {
    public:
        const_array<vec3r> r;
        box const *box;
        bool *invalid, *first_time;
        int num_particles;
        nl_data const *data;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}