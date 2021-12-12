#include "forces/primitives/sink_lj.h"

namespace xmd {
    sink_lj_vector::sink_lj_vector(int n):
        depth(n), r_min(n), r_max(n), size(n) {}

    sink_lj_array sink_lj_vector::to_array() {
        sink_lj_array a;
        a.size = size;
        a.depth = depth.to_array();
        a.r_min = r_min.to_array();
        a.r_max = r_max.to_array();
        return a;
    };

    real sink_lj::cutoff() const {
        return sink_lj::cutoff(r_max);
    }

    real sink_lj::cutoff(real r_max) {
        return 2.0 * r_max;
    }
}