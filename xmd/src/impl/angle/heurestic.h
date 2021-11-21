#pragma once
#include "types/amino_acid.h"

namespace xmd {
    enum class heurestic_angle_code: char {
        G_G, G_P, G_X,
        P_G, P_P, P_X,
        X_G, X_P, X_X
    };

    class heurestic_angle_type {
    public:
        inline heurestic_angle_type(amino_acid const& a1, amino_acid const& a2);

        inline operator char() const;

    private:
        heurestic_angle_code code;
    };

    template<typename Functor>
    class gen_heurestic_angle: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2, i3;
        field<heurestic_angle_type> type;

        inline gen_heurestic_angle(field<int> i1, field<int> i2,
            field<int> i3, field<heurestic_angle_type> type):
            i1{i1}, i2{i2}, i3{i3}, type{type} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<heurestic_angle_type>>;

        FIELDS(i1, i2, i3, type);

        template<typename F2>
        using lift = gen_heurestic_angle<compose<F2, Functor>>;
    };

    using heurestic_angle = gen_heurestic_angle<identity>;

    class compute_heurestic_angles {
    public:
        static constexpr int POLY_DEG = 6;
        xmd::list<float> poly_coeffs[POLY_DEG+1];

        xmd::list<vec3f> r;
        xmd::list<heurestic_angle> angles;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/heurestic.inl"
