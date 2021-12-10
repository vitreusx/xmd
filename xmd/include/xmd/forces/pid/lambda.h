#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/types/scalar.h>
#include <tuple>
#include <cmath>
#include <xmd/utils/math.h>

namespace xmd::pid {
    enum lambda_version {
        COSINE, ALGEBRAIC, GAUSSIAN
    };

    class lambda_func {
    public:
        real psi_0, alpha;
        lambda_version version;

        lambda_func() = default;

        lambda_func(real psi_0, real alpha, lambda_version version):
            psi_0{psi_0}, alpha{alpha}, version{version} {};

        inline bool supp(real psi) const {
            return abs(alpha * (psi - psi_0)) < M_PI;
        }

        inline std::tuple<real, real> operator()(real psi) const {
            switch (version) {
            case COSINE: {
                auto s = alpha * (psi - psi_0);
                auto val = 0.5f * cos(s) + 0.5f;
                auto deriv = -0.5f * alpha * sin(s);
                return std::make_tuple(val, deriv);
            }
            case ALGEBRAIC: {
                auto s = alpha * (psi - psi_0);
                auto t = abs(s / M_PI);
                auto x_inv = 1.0f / (2.0f * t * t - 2.0f * t - 1);
                auto val = (t * t - 2.0f * t + 1.0f) * x_inv;
                auto deriv = (2.0f * t * (t - 1.0f)) * x_inv * x_inv / M_PI;
                deriv *= (s < 0.0f ? -1.0f : 1.0f);
                return std::make_tuple(val, deriv);
            }
            default: {
                // TODO: implement the Gaussian version
                return std::make_tuple(0.0f, 0.0f);
            }
            }
        }
    };

    struct lambda_func_array {
        array<real> psi_0, alpha;
        lambda_version version;

        inline lambda_func operator[](int idx) const {
            return { psi_0[idx], alpha[idx], version };
        }
    };

    class lambda_func_vector {
    public:
        vector<real> psi_0, alpha;
        lambda_version version;

        explicit lambda_func_vector(int n = 0, lambda_version version = COSINE);
        lambda_func_array to_array();
    };
}