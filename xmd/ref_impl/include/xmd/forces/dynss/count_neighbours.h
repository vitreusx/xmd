#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::dynss {
    struct neighbor_bundle_span {
        array<int> cys_idx, other_idx;
        int size;
    };

    class count_neighbours {
    public:
        float inclusion_radius;

    public:
        vec3f_array r;
        neighbor_bundle_span bundles;
        box<vec3f> *box;
        array<int> num_neigh;

    public:
        inline void operator()() const;
    };
}