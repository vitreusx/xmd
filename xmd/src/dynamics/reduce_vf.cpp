#include "dynamics/reduce_vf.h"

namespace xmd {
    void reduce_vf::operator()() const {
#pragma omp atomic update
        *V += *thread_V;

        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx].atomic_add(thread_F[idx]);
        }
    }
}