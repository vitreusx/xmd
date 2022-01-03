#include "forces/primitives/sink_lj.h"

namespace xmd {
    real sink_lj::cutoff() const {
        return sink_lj::cutoff(r_max_);
    }

    real sink_lj::cutoff(real r_max) {
        return 2.0 * r_max;
    }
}