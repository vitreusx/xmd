#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::nl {
    class verify {
    public:
        float pad;

    public:
        vec3f_array r, orig_r;
        box<vec3f> *box, *orig_box;
        bool *invalid;
        int num_particles;

    public:
        void operator()() const;
    };
}