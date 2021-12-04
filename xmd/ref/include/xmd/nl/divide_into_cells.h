#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "nl_data.h"

namespace xmd::nl {
    class divide_into_cells {
    public:
        real cutoff, pad;

    public:
        vec3r_array r;
        box<vec3r> *box;

        nl_data *data;
        int num_particles;

    public:
        void operator()() const;
    };
}