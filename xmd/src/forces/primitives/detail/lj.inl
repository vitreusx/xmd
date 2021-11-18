#pragma once

namespace xmd {
    template<typename Functor>
    std::tuple<float, float> gen_lj<Functor>::operator()(float r_inv) const {
        auto s = r_inv * r_min, s6 = s * s * s * s * s * s, s12 = s6 * s6;
        auto V = depth * (s12 - 2.0f * s6);
        auto dV_dr = 12.0f * depth * r_inv * (s6 - s12);
        return std::make_tuple(V, dV_dr);
    }
}