#include <iostream>
#include <vcl/vectorclass.h>

void saxpy1(float const* __restrict__ x, float const* __restrict__ y, float * __restrict__ z,
        float a, int n) {
    for (int idx = 0; idx < n; ++idx) {
        z[idx] = a * x[idx] + y[idx];
    }
}

void saxpy2(Vec8f const* __restrict__ x, Vec8f const* __restrict__ y,
        Vec8f *__restrict__ z, float a, int n) {
    for (int idx = 0; idx < n; ++idx) {
        z[idx] = a * x[idx] + y[idx];
    }
}

int main() {
    return 0;
}