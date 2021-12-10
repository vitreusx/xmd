#pragma once
#include <tuple>
#include <xmd/types/scalar.h>

namespace xmd {
    class harmonic {
    public:
        real H1, H2;
        real nat_r;

        inline harmonic(real H1, real H2, real nat_r):
            H1{H1}, H2{H2}, nat_r{nat_r} {};

    public:
        inline std::tuple<real, real> operator()(real r) const {
            auto dr = r - nat_r, dr2 = dr * dr;
            auto V = (real)0.5 * dr2 * (H1 + H2 * dr2);
            auto dV_dr = dr * (H1 + (real)2.0 * H2 * dr2);
            return std::make_tuple(V, dV_dr);
        }
    };
}