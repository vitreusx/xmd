#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::dynss {
    struct neighbor_bundle_array {
        int *cys_idx, *other_idx;
        int size;
    };

    class count_neighbours {
    public:
        float inclusion_radius;

    public:
        vec3f_array *r;
        neighbor_bundle_array bundles;
        box<vec3f> *box;
        int *num_neighbours;

    public:
        inline void operator()() {
            for (int idx = 0; idx < bundles.size; ++idx) {
                auto cys_idx = bundles.cys_idx[idx];
                auto other_idx = bundles.other_idx[idx];

                auto r_cys = r[cys_idx], r_other = r[other_idx];
                auto ray = box->ray(r_cys, r_other);
                num_neigh[cys_idx] += (norm(ray) < inclusion_radius);
            }
        }
    };
}