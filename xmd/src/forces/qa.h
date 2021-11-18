#pragma once
#include "types/vec3.h"
#include "types/amino_acid.h"
#include "meta/generics.h"
#include "meta/functors.h"
#include "meta/list.h"
#include "model/box.h"
#include "forces/lj.h"

namespace xmd::qa {
    template<typename Functor>
    class gen_unformed_pair : public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;

        inline gen_unformed_pair(field<int> i1, field<int> i2) :
            i1{i1}, i2{i2} {};

    public:
        using field_types = std::tuple<field<int>, field<int>>;

        FIELDS(i1, i2);

        template<typename F2>
        using lift = gen_unformed_pair<compose<F2, Functor>>;
    };

    using unformed_pair = gen_unformed_pair<identity>;
}

namespace xmd::qa {
    template<typename Functor>
    class gen_nh_bundle : public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> iprev, i, inext;

        inline gen_nh_bundle(field<int> iprev, field<int> i, field<int> inext):
            iprev{iprev}, i{i}, inext{inext} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<int>>;

        FIELDS(iprev, i, inext);

        template<typename F2>
        using lift = gen_nh_bundle<compose<F2, Functor>>;
    };

    using nh_bundle = gen_nh_bundle<identity>;
}

namespace xmd::qa {
    template<typename Functor>
    class gen_sync_numbers : public generic_tag {
    public:
        gen_sync_numbers() = default;

        template<typename F2>
        inline auto& operator+=(gen_sync_numbers<F2> const &diff);

        template<typename F2>
        inline auto& operator-=(gen_sync_numbers<F2> const &diff);

        inline operator bool() const;

    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<short> back, side_all, side_p, side_h;

        inline gen_sync_numbers(field<short> back, field<short> side_all,
            field<short> side_p, field<short> side_h):
            back{back}, side_all{side_all}, side_p{side_p}, side_h{side_h} {};

    public:
        using field_types = std::tuple<field<short>, field<short>,
            field<short>, field<short>>;

        FIELDS(back, side_all, side_p, side_h);

        template<typename F2>
        using lift = gen_sync_numbers<compose<F2, Functor>>;
    };

    template<typename Functor>
    inline auto operator+(gen_sync_numbers<Functor> const& sync1,
        gen_sync_numbers<Functor> const& sync2);

    template<typename Functor>
    inline auto operator-(gen_sync_numbers<Functor> const& sync1,
        gen_sync_numbers<Functor> const& sync2);

    using sync_numbers = gen_sync_numbers<identity>;
}

namespace xmd::qa {
    class contact_type {
    public:
        static constexpr inline contact_type NA() {
            return 0;
        }

        static constexpr inline contact_type BB() {
            return 1;
        }

        static constexpr inline contact_type BS() {
            return 2;
        }

        static constexpr inline contact_type SB() {
            return 3;
        }

        static constexpr inline contact_type SS(
            amino_acid const &a1, amino_acid const &a2) {
            return (short)4 + (short)a1 + (short)a2 * (short)amino_acid::NUM_AA;
        }

        constexpr inline operator bool() const {
            return type != (short)NA();
        }

        inline operator short() const {
            return type;
        }

    public:
        constexpr contact_type(short type = NA()):
            type{type} {};

        short type;
    };

    enum contact_status {
        FORMING_OR_FORMED, BREAKING
    };

    template<typename Functor>
    class gen_contact : public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;
        field<contact_type> type;
        field<contact_status> status;
        field<float> ref_time;
        field<sync_numbers> sync_diff1, sync_diff2;

        inline gen_contact() = default;

        inline gen_contact(field<int> i1, field<int> i2, field<contact_type> type,
            field<contact_status> status, field<float> ref_time,
            field<sync_numbers> sync_diff1, field<sync_numbers> sync_diff2):
            i1{i1}, i2{i2}, type{type}, status{status}, ref_time{ref_time},
            sync_diff1{sync_diff1}, sync_diff2{sync_diff2} {};

    public:
        using field_types = std::tuple<field<int>, field<int>, field<contact_type>,
            field<contact_status>, field<float>, field<sync_numbers>, field<sync_numbers>>;

        FIELDS(i1, i2, type, status, ref_time, sync_diff1, sync_diff2);

        template<typename F2>
        using lift = gen_contact<compose<F2, Functor>>;
    };

    using contact = gen_contact<identity>;
}

namespace xmd::qa {
    template<typename Functor>
    class gen_candidate: public generic_tag {
    public:
        template<typename T>
        using field = typename Functor::template type<T>;

        field<int> i1, i2;
        field<contact_type> type;
        field<sync_numbers> sync_diff1, sync_diff2;

        inline gen_candidate() = default;

        inline gen_candidate(field<int> i1, field<int> i2, field<contact_type> type,
            field<sync_numbers> sync_diff1, field<sync_numbers> sync_diff2):
            i1{i1}, i2{i2}, type{type}, sync_diff1{sync_diff1}, sync_diff2{sync_diff2} {};

    public:
        using field_types = std::tuple<field<int>, field<int>,
            field<contact_type>, field<sync_numbers>, field<sync_numbers>>;

        FIELDS(i1, i2, type, sync_diff1, sync_diff2);

        template<typename F2>
        using lift = gen_candidate<compose<F2, Functor>>;
    };

    using candidate = gen_candidate<identity>;
}

namespace xmd::qa {
    class precompute_nh {
    public:
        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<nh_bundle> bundles;

        xmd::list<vec3f> n, h;

    public:
        inline void operator()();
    };

    class sift_formation_candidates {
    public:
        float min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        xmd::list<float> r_min;
        xmd::list<polarization_type> ptype;

        xmd::list<vec3f> r, n, h;
        boxf *box;
        xmd::list<amino_acid> atype;
        xmd::list<sync_numbers> sync_ns;
        xmd::list<unformed_pair> pairs;

        xmd::list<candidate> candidates;

    public:
        inline void operator()();
    };

    class process_candidates {
    public:
        xmd::list<candidate> candidates;
        xmd::list<sync_numbers> sync_ns;

        float *t;
        xmd::list<contact> contacts;

    public:
        inline void operator()();
    };

    class process_contacts {
    public:
        xmd::list<lj> lj_forces;
        float cycle_time, cycle_time_inv, breaking_factor;

        xmd::list<vec3f> r;
        boxf *box;
        xmd::list<contact> contacts;
        xmd::list<size_t> marked_for_erasure;

        xmd::list<vec3f> F;
        float *V, *t;
        xmd::list<sync_numbers> sync_ns;

    public:
        inline void operator()();
    };
}

#include "detail/qa.inl"
