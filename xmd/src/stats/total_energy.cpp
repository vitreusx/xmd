#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        real K_ = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            K_ += (real)0.5 * mass[idx] * norm_squared(v[idx]);
        }
        *K = K_;
        *E = *V + K_;
    }
}