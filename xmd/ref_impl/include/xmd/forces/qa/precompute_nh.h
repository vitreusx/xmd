#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::qa {
    struct nh_bundle_span {
        array<int> iprev, icur, inext;
        int size;
    };

    class precompute_nh {
    public:
        vec3f_array r, n, h;
        box<vec3f> *box;
        nh_bundle_span bundles;

    public:
        void operator()() const;
    };
}