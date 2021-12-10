#pragma once
#include <xmd/utils/math.h>
#include <xmd/types/scalar.h>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include "lj.h"

namespace xmd {
    class sink_lj {
    public:
        real depth, r_min, r_max;

        sink_lj() = default;

        inline sink_lj(real depth, real r_min, real r_max):
            depth{depth}, r_min{r_min}, r_max{r_max} {};

        inline sink_lj(lj const& lj_):
            depth{lj_.depth}, r_min{lj_.r_min}, r_max{lj_.r_min} {};

        inline std::tuple<real, real> operator()(real r, real r_inv) const {
            auto r_eff = (r < r_min ? r_min : (r < r_max ? r : r_max));
            auto s = r_inv * r_eff, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth * (s12 - 2.0f * s6);
            auto dV_dr = 12.0f * depth * r_inv * (s6 - s12);
            dV_dr = (r_min < r && r < r_max) ? 0 : dV_dr;
            return std::make_tuple(V, dV_dr);
        }
    };

    struct sink_lj_array {
        array<real> depth, r_min, r_max;
        int size;

        inline sink_lj operator[](int idx) const {
            return { depth[idx], r_min[idx], r_max[idx] };
        }
    };

    class sink_lj_vector {
    public:
        vector<real> depth, r_min, r_max;
        int size;

        explicit sink_lj_vector(int n);
        sink_lj_array to_array();
    };
}
