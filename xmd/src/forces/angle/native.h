#pragma once
#include "types/amino_acid.h"

namespace xmd {
    template<typename Functor>
    class gen_native_angle {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2, i3;
        field<float> native_theta;

        inline gen_native_angle(field<int> i1, field<int> i2,
            field<int> i3, field<float> native_theta):
            i1{i1}, i2{i2}, i3{i3}, native_theta{native_theta} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<heurestic_angle_type>>;

        FIELDS(i1, i2, i3, native_theta);

        template<typename F2>
        using lift = gen_native_angle<compose<F2, Functor>>;
    };

    using native_angle = gen_native_angle<identity>;

    class compute_native_angles {
    public:
        float k;
        xmd::list<vec3f> r;
        xmd::list<native_angle> angles;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/native.inl"
