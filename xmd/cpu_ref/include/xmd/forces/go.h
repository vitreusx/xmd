#pragma once

namespace xmd {
    template<typename Functor>
    class gen_go_contact {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;
        field<float> native_dist;

        gen_go_contact(field<int> i1, field<int> i2, field<float> native_dist):
            i1{i1}, i2{i2}, native_dist{native_dist} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<float>>;

        FIELDS(i1, i2, native_dist);

        template<typename F2>
        using lift = gen_go_contact<compose<F2, Functor>>;
    };

    using go_contact = gen_go_contact<identity>;

    class compute_go {
    public:
        float depth;

        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<go_contact> contacts;

        xmd::list<vec3f> F;
        float *V;

    public:
        void operator()();
    };
}

#include "detail/go.inl"
