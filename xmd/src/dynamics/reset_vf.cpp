#include "dynamics/reset_vf.h"

namespace xmd {
    void reset_vf::operator()() const {
        *V = (real)0.0;
        for (int idx = 0; idx < F.size(); ++idx)
            F[idx] = vec3r::Zero();
    }

    void reset_vf::omp_async() const {
        *V = (real)0.0;
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < F.size(); ++idx)
            F[idx] = vec3r::Zero();
    }
}