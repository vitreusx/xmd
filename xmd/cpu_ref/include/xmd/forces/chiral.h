#pragma once
#include <xmd/meta/generics.h>

namespace xmd {
    template<typename Functor>
    class gen_chiral_quad: public generic_tag {
    public:
        USING_FIELD(Functor);

        field<int> i1, i2, i3, i4;
        field<float> native_chirality, native_d0_rncube;

        inline gen_chiral_quad() = default;

        inline gen_chiral_quad(field<int> i1, field<int> i2, field<int> i3,
            field<int> i4, field<float> native_chirality,
            field<float> native_d0_rncube):
            i1{i1}, i2{i2}, i3{i3}, i4{i4}, native_chirality{native_chirality},
            native_d0_rncube{native_d0_rncube} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<int>, field<float>, field<float>>;
        FIELDS(i1, i2, i3, i4, native_chirality, native_d0_rncube);

        USING_LIFT(gen_chiral_quad, Functor);
        USING_LIFTFN;
    };

    using chiral_quad = gen_chiral_quad<identity>;

    class compute_chiral_forces {
    public:
        float e_chi;
        xmd::list<vec3f> r;
        xmd::list<chiral_quad> quads;

        xmd::list<vec3f> F;
        float *V;

        inline chiral_quad make_quad(int i1, int i2, int i3, int i4);

    public:
        inline void operator()();
    };
}

#include "detail/chiral.inl"
