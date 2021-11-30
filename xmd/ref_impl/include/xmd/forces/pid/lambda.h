#pragma once
#include <xmd/types/array.h>
#include <tuple>
#include <cmath>
#include <xmd/math.h>

namespace xmd::pid {
    enum lambda_version {
        COSINE, ALGEBRAIC, GAUSSIAN
    };

    class lambda_func {
    public:
        float psi_0, alpha;
        lambda_version version;

        lambda_func() = default;

        lambda_func(float psi_0, float alpha, lambda_version version):
            psi_0{psi_0}, alpha{alpha}, version{version} {};

        inline bool supp(float psi) const {
            return abs(alpha * (psi - psi_0)) < M_PI;
        }

        inline std::tuple<float, float> operator()(float psi) const {
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
        array<float> psi_0, alpha;
        lambda_version version;

        inline lambda_func operator[](int idx) const {
            return { psi_0[idx], alpha[idx], version };
        }
    };
}