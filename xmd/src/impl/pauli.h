#pragma once

namespace xmd {
    template<typename Functor>
    class gen_pauli_pair {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;

        inline gen_pauli_pair(field<int> i1, field<int> i2):
            i1{i1}, i2{i2} {};

    public:
        using field_types = std::tuple<field<int>, field<int>>;

        FIELDS(i1, i2);

        template<typename F2>
        using lift = gen_pauli_pair<compose<F2, Functor>>;
    };

    using pauli_pair = gen_pauli_pair<identity>;

    class compute_pauli {
    public:
        float depth, r_excl;

        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<pauli_pair> pairs;

        xmd::list<vec3f> F;
        float *V;

    public:
        void operator()();
    };
}

#include "detail/pauli.inl"
