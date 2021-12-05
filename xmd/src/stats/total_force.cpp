#include "stats/total_force.h"

namespace xmd {
    void compute_total_force::operator()() {
        vec3r res = vec3r::Zero();
        for (int idx = 0; idx < F.size(); ++idx) {
            res += F[idx];
        }
        *total_F = res;
    }
}