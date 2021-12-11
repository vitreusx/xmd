#include "forces/primitives/lj.h"

namespace xmd {
    lj_vector::lj_vector(int n):
        r_min(n), depth(n), size(n) {}

    lj_array lj_vector::to_array() {
        lj_array a;
        a.size = size;
        a.r_min = r_min.to_array();
        a.depth = depth.to_array();
        return a;
    };

    real lj::cutoff() const {
        return 2.0 * r_min;
    }
}