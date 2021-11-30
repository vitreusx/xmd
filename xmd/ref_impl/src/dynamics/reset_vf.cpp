#include "dynamics/reset_vf.h"

namespace xmd {
    void reset_vf::operator()() const {
        *V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3f::Zero();
        }
    }
}