#pragma once
#include <xmd/types/scalar.h>

namespace xmd {
    class shifted_lj {
    public:
        real depth, r_min;

        shifted_lj() = default;

        inline shifted_lj(real depth, real r_min):
            depth{depth}, r_min{r_min} {};

        inline std::tuple<real, real> operator()(real r_inv) const {
            auto s = r_inv * r_min, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth * (s12 - (real)2.0 * s6 + (real)1.0);
            auto dV_dr = (real)12.0 * depth * r_inv * (s6 - s12);
            V = clamp<real>(V, -1.0e3, 1.0e3);
            dV_dr = clamp<real>(dV_dr, -1.0e3, 1.0e3);
            return std::make_tuple(V, dV_dr);
        }
    };
}