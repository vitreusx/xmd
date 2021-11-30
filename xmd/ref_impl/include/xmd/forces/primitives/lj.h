#pragma once
#include <tuple>
#include <xmd/math.h>

namespace xmd {
    class lj {
    public:
        float depth, r_min;

        lj() = default;

        inline lj(float depth, float r_min):
            depth{depth}, r_min{r_min} {};

        inline std::tuple<float, float> operator()(float r_inv) const {
            auto s = r_inv * r_min, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth * (s12 - 2.0f * s6);
            auto dV_dr = 12.0f * depth * r_inv * (s6 - s12);
            return std::make_tuple(V, dV_dr);
        }
    };

    struct lj_array {
        float *depth, *r_min;
        int size;

        inline lj operator[](int idx) {
            return { depth[idx], r_min[idx] };
        }
    };
}
