#include <iostream>

void saxpy1(float * __restrict__ x, float * __restrict__ y, float * __restrict__ z,
        float a, int n) {
    for (int idx = 0; idx < n; ++idx) {
        z[idx] = a * x[idx] + y[idx];
    }
}

int main() {
    return 0;
}