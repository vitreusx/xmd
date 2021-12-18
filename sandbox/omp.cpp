#include <xmd/types/vec3.h>
using namespace xmd;

void saxpy(vec3r_array const& x, vec3r_array const& y, vec3r_array const& z, real a, int n) {
    for (int idx = 0; idx < n; ++idx) {
        z[idx] += a * x[idx] + y[idx];
    }
}

int main() {
    return 0;
}