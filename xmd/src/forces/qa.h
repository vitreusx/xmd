#pragma once
#include "types/vector3.h"
#include "types/amino_acid.h"
#include "types/array.h"
#include "types/functors.h"
#include "model/box.h"

namespace xmd::qa {
    struct unformed_pair {
        int i1, i2;
    };
}

namespace xmd {
    template<>
    class array<qa::unformed_pair> {
    public:
        inline qa::unformed_pair operator[](size_t idx) const;
        inline size_t size() const;

    public:
        xmd::array<int> i1, i2;
        size_t size_;
    };
}

namespace xmd::qa {
    struct nh_bundle {
        int iprev, i, inext;
    };
}

namespace xmd {
    template<>
    class array<qa::nh_bundle> {
    public:
        inline qa::nh_bundle operator[](size_t idx) const;
        inline size_t size() const;

    public:
        xmd::array<int> iprev, i, inext;
        size_t size_;
    };
}

namespace xmd::qa {
    template<template<typename> typename field>
    class gen_sync_numbers {
    public:
        inline gen_sync_numbers operator+(gen_sync_numbers const &diff) const;

        inline gen_sync_numbers &operator+=(gen_sync_numbers const &diff);

        inline gen_sync_numbers operator-(gen_sync_numbers const &diff) const;

        inline gen_sync_numbers &operator-=(gen_sync_numbers const &diff);

        inline operator bool() const;

    public:
        gen_sync_numbers() = default;

        field<short> back, side_all, side_p, side_h;
    };

    using sync_numbers = gen_sync_numbers<xmd::identity>;
    using sync_numbers_ref = gen_sync_numbers<std::add_lvalue_reference>;
}

namespace xmd {
    template<>
    class array<qa::sync_numbers> {
    public:
        inline qa::sync_numbers operator[](size_t idx) const;
        inline qa::sync_numbers_ref operator[](size_t idx);
        inline size_t size() const;

    public:
        xmd::array<short> back, side_all, side_p, side_h;
        size_t size_;
    };
}

namespace xmd::qa {
    class contact_type {
    public:
        static inline contact_type None();

        static inline contact_type BB();

        static inline contact_type BS();

        static inline contact_type SB();

        static inline contact_type
        SS(amino_acid const &a1, amino_acid const &a2);

        inline operator bool() const;

        inline operator short() const;

    public:
        contact_type() = default;

        contact_type(short type);

        short type;
    };

    enum contact_status {
        FORMING_OR_FORMED, BREAKING
    };

    template<template<typename> typename field>
    struct gen_contact {
        field<int> i1, i2;
        field<contact_type> type;
        field<contact_status> status;
        field<float> ref_time;
        field<sync_numbers> sync_diff1, sync_diff2;
    };

    using contact = gen_contact<xmd::identity>;
    using contact_ref = gen_contact<std::add_lvalue_reference>;
}

namespace xmd {
    template<>
    class array<qa::contact> {
    public:
        inline qa::contact operator[](size_t idx) const;
        inline qa::contact_ref operator[](size_t idx);
        inline size_t size() const;

    public:
        xmd::array<int> i1, i2;
        xmd::array<qa::contact_type> type;
        xmd::array<qa::contact_status> status;
        xmd::array<float> ref_time;
        xmd::array<qa::sync_numbers> sync_diff1, sync_diff2;
        size_t size_;
    };
}

namespace xmd::qa {
    struct candidate {
        int i1, i2;
        contact_type type;
        sync_numbers sync_diff1, sync_diff2;
    };
}

namespace xmd {
    template<>
    class array<qa::candidate> {
    public:
        inline qa::candidate operator[](size_t idx) const;
        inline size_t size() const;

    public:
        xmd::array<int> i1, i2;
        xmd::array<qa::contact_type> type;
        xmd::array<qa::sync_numbers> sync_diff1, sync_diff2;
        size_t size_;
    };
}

namespace xmd::qa {
    class precompute_nh {
    public:
        array<vector3f> r;
        boxf *box;
        array<nh_bundle> bundles;

        array<vector3f> n, h;

    public:
        inline void operator()() const;
    };

    class sift_formation_candidates {
    public:
        float min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        array<float> r_min;
        array<polarization_type> ptype;

        array<vector3f> r, n, h;
        boxf *box;
        array<amino_acid> atype;
        array<sync_numbers> sync_ns;
        array<unformed_pair> pairs;

        array<candidate> candidates;

    public:
        inline void operator()() const;
    };

    class process_candidates {
    public:
        array<candidate> candidates;
        array<sync_numbers> sync_ns;

        float *t;
        array<contact> contacts;

    public:
        inline void operator()() const;
    };

    class process_contacts {
    public:
        array<float> r_min_inv, lj_depth;
        float cycle_time, cycle_time_inv;

        array<vector3f> r;
        boxf *box;
        array<contact> contacts;

        array<vector3f> F;
        float *V, *t;

    public:
        inline void operator()() const;
    };
}

#include "detail/qa.inl"
