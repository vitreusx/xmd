#include <iostream>
#include <xmd/types/vec3_.h>
using namespace xmd;

using vec3f_const_ptr = vec3_const_ptr<float>;
using vec3f_ptr = vec3_ptr<float>;

void saxpy1(float const *xx, float const *xy, float const *xz,
    float const *yx, float const *yy, float const *yz,
    float *zx, float *zy, float *zz,
    float a, int n) {

    for (int i = 0; i < n; ++i) {
        zx[i] = a * xx[i] + yx[i];
        zy[i] = a * xy[i] + yy[i];
        zz[i] = a * xz[i] + yz[i];
    }
}

void saxpy2(vec3f_const_ptr x, vec3f_const_ptr y, vec3f_ptr z, float a, int n) {
    for (int i = 0; i < n; ++i) {
        z[i] = a * x[i] + y[i];
    }
}

int main() {
    return 0;
}