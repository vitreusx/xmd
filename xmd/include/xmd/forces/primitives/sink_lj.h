#pragma once
#include <xmd/utils/math.h>
#include <xmd/types/scalar.h>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include "lj.h"

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() sink_lj
#define FIELDS() real,depth,real,r_min,real,r_max

GEN_EXPR()

namespace xmd {
    class sink_lj: public sink_lj_expr<sink_lj> {
    public:
        sink_lj() = default;

        sink_lj(real depth, real r_min, real r_max):
            depth_{depth}, r_min_{r_min}, r_max_{r_max} {};

        explicit sink_lj(lj const& lj_):
            depth_{lj_.depth()}, r_min_{lj_.r_min()}, r_max_{lj_.r_min()} {};

        INST_CTORS()
        INST_ASSIGN_COPY()
        INST_ASSIGN_EXPR()

        inline std::tuple<real, real> operator()(real r, real r_inv) const {
            auto r_eff = (r < r_min_ ? r_min_ : (r < r_max_ ? r : r_max_));
            auto s = r_inv * r_eff, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth_ * (s12 - 2.0f * s6);
            auto dV_dr = 12.0f * depth_ * r_inv * (s6 - s12);
            dV_dr = (r_min_ < r && r < r_max_) ? 0 : dV_dr;
            V = clamp<real>(V, -1.0e3, 1.0e3);
            dV_dr = clamp<real>(dV_dr, -1.0e3, 1.0e3);
            return std::make_tuple(V, dV_dr);
        }

        real cutoff() const;
        static real cutoff(real r_max);

        INST_LAZY_FIELDS()

    private:
        INST_FIELDS()
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