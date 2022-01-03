#include "forces/primitives/lj.h"

namespace xmd {
    real lj::cutoff() const {
        return lj::cutoff(r_min_);
    }

    real lj::cutoff(real r_min) {
        return 2.0 * r_min;
    }
}