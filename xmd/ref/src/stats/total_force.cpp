#include "stats/total_force.h"

namespace xmd {
    void compute_total_force::operator()() {
        vec3f res = vec3f::Zero();
        for (int idx = 0; idx < F.size(); ++idx) {
            res += F[idx];
        }
        *total_F = res;
    }
}