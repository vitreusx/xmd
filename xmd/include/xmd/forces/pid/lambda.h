#pragma once
#include <gentypes/gentype.h>
#include <xmd/types/scalar.h>
#include <tuple>
#include <cmath>
#include <xmd/utils/math.h>

namespace xmd::pid {
    enum lambda_version {
        COSINE, ALGEBRAIC, GAUSSIAN
    };
}

#define NAMESPACE(...) xmd,pid,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() lambda_func
#define FIELDS() real,psi_0,real,alpha,lambda_version,version

GEN_EXPR()

namespace xmd::pid {
    class lambda_func: public lambda_func_expr<lambda_func> {
    public:
        lambda_func() = default;

        INST_CTORS()
        INST_ASSIGN_COPY()
        INST_ASSIGN_EXPR()

        inline bool supp(real psi) const {
            return abs(alpha_ * (psi - psi_0_)) < M_PI;
        }

        inline std::tuple<real, real> operator()(real psi) const {
            switch (version_) {
            case COSINE: {
                auto s = alpha_ * (psi - psi_0_);
                auto val = 0.5f * cos(s) + 0.5f;
                auto deriv = -0.5f * alpha_ * sin(s);
                return std::make_tuple(val, deriv);
            }
            case ALGEBRAIC: {
                auto s = alpha_ * (psi - psi_0_);
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

        INST_LAZY_FIELDS()

    private:
        INST_FIELDS()
    };

    struct lambda_func_array {
        array<real> psi_0, alpha;
        lambda_version version;

        inline lambda_func operator[](int idx) const {
            return { psi_0[idx], alpha[idx], version };
        }
    };
}

GEN_REF()
GEN_CONST_REF()
GEN_PTR()
GEN_CONST_PTR()
GEN_CONST_SPAN()
GEN_SPAN()
GEN_MEMORY()
GEN_ALLOCATOR()
GEN_VECTOR()
GEN_SET()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE