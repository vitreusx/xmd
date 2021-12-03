#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "nl_data.h"

namespace xmd::nl {
    class divide_into_cells {
    public:
        float cutoff, pad;

    public:
        vec3f_array r;
        box<vec3f> *box;

        nl_data *data;
        int num_particles;

    public:
        void operator()() const;
    };
}