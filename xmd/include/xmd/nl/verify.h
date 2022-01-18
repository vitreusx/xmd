#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

#include "data.h"

namespace xmd::nl {
    class verify {
    public:
        const_array<vec3r> r;
        box const *box;
        bool *invalid, *first_time;
        int num_particles;
        nl_data const *data;



    public:
        void operator()() const;
    };
}