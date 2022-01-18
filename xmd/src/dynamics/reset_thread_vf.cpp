#include "dynamics/reset_thread_vf.h"

namespace xmd {
    void reset_thread_vf::operator()() const {
        *thread_V = (real)0.0;
        for (int idx = 0; idx < thread_F.size(); ++idx)
            thread_F[idx] = vec3r::Zero();
    }
}