#pragma once
#include <xmd/math.h>

namespace xmd {
    class sink_lj {
    public:
        float depth, r_min, r_max;

        sink_lj() = default;

        inline sink_lj(float depth, float r_min, float r_max):
            depth{depth}, r_min{r_min}, r_max{r_max} {};

        inline std::tuple<float, float> operator()(float r, float r_inv) const {
            auto r_eff = (r < r_min ? r_min : (r < r_max ? r : r_max));
            auto s = r_inv * r_eff, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth * (s12 - 2.0f * s6);
            auto dV_dr = 12.0f * depth * r_inv * (s6 - s12);
            return std::make_tuple(V, dV_dr);
        }
    };

    struct sink_lj_array {
        float *depth, *r_min, *r_max;
        int size;

        inline sink_lj operator[](int idx) const {
            return { depth[idx], r_min[idx], r_max[idx] };
        }
    };
}
