#pragma once

namespace xmd {
    template<typename Functor>
    class gen_sink_lj {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<float> depth, r_min, r_max;

        inline gen_sink_lj(field<float> depth, field<float> r_min,
            field<float> r_max): depth{depth}, r_min{r_min}, r_max{r_max} {};

        inline std::tuple<float, float> operator()(float r, float r_inv) const;

    public:
        using field_types = std::tuple<field<float>, field<float>, field<float>>;

        FIELDS(depth, r_min, r_max);

        template<typename F2>
        using lift = gen_sink_lj<compose<F2, Functor>>;
    };

    using sink_lj = gen_sink_lj<identity>;
}

#include "detail/sink_lj.inl"
