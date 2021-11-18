#pragma once
#include "meta/generics.h"

namespace xmd {
    template<typename Functor>
    class gen_tether_pair: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;
        field<float> dist0;

        inline gen_tether_pair(field<int> i1, field<int> i2, field<float> dist0):
            i1{i1}, i2{i2}, dist0{dist0} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<float>>;

        FIELDS(i1, i2, dist0);

        template<typename F2>
        using lift = gen_tether_pair<compose<F2, Functor>>;
    };

    using tether_pair = gen_tether_pair<identity>;

    class compute_tether_forces {
    public:
        float H1, H2;

        xmd::list<vec3f> r;
        xmd::list<tether_pair> pairs;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/tether.inl"
