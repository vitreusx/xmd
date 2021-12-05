#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        real K = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            K += (real)0.5 * mass[idx] * norm(v[idx]);
        }
        *E = *V + K;
    }
}