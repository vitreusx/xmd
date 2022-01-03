#pragma once
#include <tuple>


#include <xmd/utils/math.h>
#include <xmd/types/scalar.h>
#include <gentypes/gentype.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() lj
#define FIELDS() xmd::real,depth,xmd::real,r_min

GEN_EXPR()

namespace xmd {
    class lj: public lj_expr<lj> {
    public:
        lj() = default;

        INST_CTORS()
        INST_ASSIGN_COPY()
        INST_ASSIGN_EXPR()

        inline std::tuple<real, real> operator()(real r_inv) const {
            auto s = r_inv * r_min_, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth_ * (s12 - (real)2.0 * s6);
            auto dV_dr = (real)12.0 * depth_ * r_inv * (s6 - s12);
            V = clamp<real>(V, -1.0e3, 1.0e3);
            dV_dr = clamp<real>(dV_dr, -1.0e3, 1.0e3);
            return std::make_tuple(V, dV_dr);
        }

        real cutoff() const;
        static real cutoff(real r_min);

        INST_LAZY_FIELDS()

    private:
        INST_FIELDS()
    };
}

GEN_REF()
GEN_CONST_REF()
GEN_PTR()
GEN_CONST_PTR()
GEN_SPAN()
GEN_CONST_SPAN()
GEN_MEMORY()
GEN_ALLOCATOR()
GEN_VECTOR()
GEN_SET()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE