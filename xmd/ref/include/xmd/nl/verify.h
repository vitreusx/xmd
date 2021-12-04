#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::nl {
    class verify {
    public:
        real pad;

    public:
        vec3r_array r, orig_r;
        box<vec3r> *box, *orig_box;
        bool *invalid;
        int num_particles;

    public:
        void operator()() const;
    };
}