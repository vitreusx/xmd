#include "forces/dynss/count_neighbours.h"

namespace xmd::dynss {

    void count_neighbours::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto cys_idx = bundles.cys_idx[idx];
            auto other_idx = bundles.other_idx[idx];

            auto r_cys = r[cys_idx], r_other = r[other_idx];
            auto ray = box->ray(r_cys, r_other);
            num_neigh[cys_idx] += (norm(ray) < inclusion_radius);
        }
    }
}