#include "forces/primitives/harmonic.h"

namespace xmd {
    real harmonic::cutoff(real nat_r) {
        return 2.0 * nat_r;
    }
}