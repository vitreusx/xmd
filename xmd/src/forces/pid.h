#pragma once
#include "forces/primitives/lj.h"
#include "forces/primitives/sink_lj.h"

namespace xmd {
    template<typename Functor>
    class gen_pid_bundle {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1p, i1, i1n;
        field<int> i2p, i2, i2n;
        field<char> type;

        inline gen_pid_bundle(field<int> i1p, field<int> i1, field<int> i1n,
            field<int> i2p, field<int> i2, field<int> i2n, field<char> type):
            i1p{i1p}, i1{i1}, i1n{i1n}, i2p{i2p}, i2{i2}, i2n{i2n}, type{type} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>,
            field<int>, field<int>, field<int>, field<char>>;

        FIELDS(i1p, i1, i1n, i2p, i2, i2n, type);

        template<typename F2>
        using lift = gen_pid_bundle<compose<F2, Functor>>;
    };

    using pid_bundle = gen_pid_bundle<identity>;

    template<typename Functor, bool CosineVersion = true>
    class gen_lambda_func {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<float> psi_0, alpha;

        inline gen_lambda_func(field<float> psi_0, field<float> alpha):
            psi_0{psi_0}, alpha{alpha} {};

        inline bool supports(float psi) const;
        inline std::tuple<float, float> operator()(float psi) const;

    public:
        using field_types = std::tuple<field<float>, field<float>>;

        FIELDS(psi_0, alpha);

        template<typename F2>
        using lift = gen_lambda_func<compose<F2, Functor>, CosineVersion>;
    };

    template<bool CosineVersion = true>
    using lambda_func = gen_lambda_func<identity, CosineVersion>;

    class compute_pid {
    public:
        lambda_func<> bb_plus_lam, bb_minus_lam;
        lj bb_plus_lj, bb_minus_lj;

        xmd::list<lambda_func<>> ss_lams;
        xmd::list<sink_lj> ss_sink_ljs;

        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<pid_bundle> bundles;

        xmd::list<vec3f> F;
        float *V;

    public:
        void operator()();

    private:
        inline void per_bb_term(lj const& lj, lambda_func<> const& lam, float psi[2],
            float r_inv, float& A, float& B, float& C) const;

        inline void per_ss_term(sink_lj const& lj, lambda_func<> const& lam, float psi[2],
            float r, float r_inv, float& A, float& B, float& C) const;
    };
}

#include "detail/pid.inl"
