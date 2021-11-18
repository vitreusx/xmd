#pragma once
#include "types/vec3.h"
#include "meta/generics.h"

namespace xmd {
    template<typename Functor>
    class gen_lj: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<float> depth, r_min;
        gen_lj(field<float> depth, field<float> r_min):
            depth{depth}, r_min{r_min} {};

        inline std::tuple<float, float> operator()(float rnorm) const;

    public:
        using field_types = std::tuple<field<float>, field<float>>;

        FIELDS(depth, r_min);

        template<typename F2>
        using lift = gen_lj<compose<F2, Functor>>;
    };

    using lj = gen_lj<identity>;
}

#include "detail/lj.inl"
