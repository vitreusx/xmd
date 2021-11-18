#pragma once

namespace xmd {
    template<typename Functor>
    class gen_es_pair: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;
        field<float> q1_q2;

        inline gen_es_pair(field<int> i1, field<int> i2, field<float> q1_q2):
            i1{i1}, i2{i2}, q1_q2{q1_q2} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<float>>;

        FIELDS(i1, i2, q1_q2);

        template<typename F2>
        using lift = gen_es_pair<compose<F2, Functor>>;
    };

    using es_pair = gen_es_pair<identity>;
}