#pragma once

namespace xmd {
    enum class heurestic_dihedral_code: char {
        G_G, G_P, G_X,
        P_G, P_P, P_X,
        X_G, X_P, X_X
    };

    class heurestic_dihedral_type {
    public:
        inline heurestic_dihedral_type(amino_acid const& a2, amino_acid const& a3);

        inline operator char() const;

    private:
        heurestic_dihedral_code code;
    };

    template<typename Functor>
    class gen_heurestic_dihedral: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2, i3, i4;
        field<heurestic_dihedral_type> type;

        inline gen_heurestic_dihedral(field<int> i1, field<int> i2,
            field<int> i3, field<int> i4, field<heurestic_dihedral_type> type):
            i1{i1}, i2{i2}, i3{i3}, i4{i4}, type{type} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<int>, field<heurestic_dihedral_type>>;

        FIELDS(i1, i2, i3, i4, type);

        template<typename F2>
        using lift = gen_heurestic_dihedral<compose<F2, Functor>>;
    };

    using heurestic_dihedral = gen_heurestic_dihedral<identity>;

    class compute_heurestic_dihedrals {
    public:
        static constexpr int NUM_TERMS = 6;
        xmd::list<float> coeffs[NUM_TERMS];

        xmd::list<vec3f> r;
        xmd::list<heurestic_dihedral> dihedrals;

        xmd::list<vec3f> F;
        float *V;

    public:
        inline void operator()();
    };
}

#include "detail/heurestic.inl"
