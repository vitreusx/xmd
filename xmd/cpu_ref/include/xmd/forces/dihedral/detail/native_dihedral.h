#pragma once

namespace xmd {
    template<typename Functor>
    class gen_native_dihedral: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2, i3, i4;
        field<float> native_phi;

        inline gen_native_dihedral(field<int> i1, field<int> i2,
            field<int> i3, field<int> i4, field<float> native_phi):
            i1{i1}, i2{i2}, i3{i3}, i4{i4}, native_phi{native_phi} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<int>, field<float>>;

        FIELDS(i1, i2, i3, i4, native_phi);

        template<typename F2>
        using lift = gen_native_dihedral<compose<F2, Functor>>;
    };

    using native_dihedral = gen_native_dihedral<identity>;
}