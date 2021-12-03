#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        auto K = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            K += 0.5f * mass[idx] * norm(v[idx]);
        }
        *E = *V + K;
    }
}