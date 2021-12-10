#include "forces/pid/lambda.h"

namespace xmd::pid {
    lambda_func_vector::lambda_func_vector(int n, lambda_version version):
        psi_0(n), alpha(n), version(version) {};

    lambda_func_array lambda_func_vector::to_array() {
        lambda_func_array a;
        a.version = version;
        a.psi_0 = psi_0.to_array();
        a.alpha = alpha.to_array();
        return a;
    }
}