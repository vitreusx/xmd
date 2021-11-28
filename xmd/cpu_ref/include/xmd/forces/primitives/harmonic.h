#pragma once
#include <tuple>

namespace xmd {
    class harmonic {
    public:
        float H1, H2;
        float nat_r;

        harmonic(float H1, float H2, float nat_r):
            H1{H1}, H2{H2}, nat_r{nat_r} {};

    public:
        inline std::tuple<float, float> operator()(float r) const {
            auto dr = r - nat_r, dr2 = dr * dr;
            auto V = dr2 * (H1 + H2 * dr2);
            auto dV_dr = dr * (2.0f * H1 + 4.0f * H2 * dr2);
            return std::make_tuple(V, dV_dr);
        }
    };
}