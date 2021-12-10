#pragma once
#include <tuple>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/utils/math.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class lj {
    public:
        real depth, r_min;

        lj() = default;

        inline lj(real depth, real r_min):
            depth{depth}, r_min{r_min} {};

        inline std::tuple<real, real> operator()(real r_inv) const {
            auto s = r_inv * r_min, s6 = ipow<6>(s), s12 = s6 * s6;
            auto V = depth * (s12 - 2.0f * s6);
            auto dV_dr = 12.0f * depth * r_inv * (s6 - s12);
            return std::make_tuple(V, dV_dr);
        }
    };

    struct lj_array {
        array<real> depth, r_min;
        int size;

        inline lj operator[](int idx) const {
            return { depth[idx], r_min[idx] };
        }
    };

    class lj_vector {
    public:
        vector<real> depth, r_min;
        int size;

        explicit lj_vector(int n);
        lj_array to_array();
    };
}
