#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::nl {
    class verify {
    public:

    public:
        vec3r_array r, orig_r;
        box<vec3r> *box, *orig_box;
        bool *invalid, *first_time;
        int num_particles;
        real pad;

    public:
        void operator()() const;
    };
}