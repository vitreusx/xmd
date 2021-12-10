#pragma once
#include <cstdint>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>

namespace xmd::qa {
    class sync_data_cref {
    public:
        inline sync_data_cref(int8_t const& back, int8_t const& side_all,
            int8_t const& side_polar, int8_t const& side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

    public:
        int8_t const &back, &side_all, &side_polar, &side_hydrophobic;
    };

    class sync_data {
    public:
        sync_data() = default;

        inline sync_data(int8_t back, int8_t side_all, int8_t side_polar,
            int8_t side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

        inline sync_data(sync_data_cref const& other):
            back{other.back}, side_all{other.side_all},
            side_polar{other.side_polar}, side_hydrophobic{other.side_hydrophobic} {};

        inline operator sync_data_cref() {
            return { back, side_all, side_polar, side_hydrophobic };
        }

        inline bool is_valid() const {
            return (back >= 0) && (side_all >= 0) && (side_polar >= 0) &&
                   (side_hydrophobic >= 0);
        }

    public:
        int8_t back = 0, side_all = 0, side_polar = 0, side_hydrophobic = 0;
    };

    inline sync_data operator+(sync_data const& s1, sync_data const& s2) {
        return { static_cast<int8_t>(s1.back + s2.back),
                 static_cast<int8_t>(s1.side_all + s2.side_all),
                 static_cast<int8_t>(s1.side_polar + s2.side_polar),
                 static_cast<int8_t>(s1.side_hydrophobic + s2.side_hydrophobic) };
    }

    inline sync_data operator-(sync_data const& s1, sync_data const& s2) {
        return { static_cast<int8_t>(s1.back - s2.back),
                 static_cast<int8_t>(s1.side_all - s2.side_all),
                 static_cast<int8_t>(s1.side_polar - s2.side_polar),
                 static_cast<int8_t>(s1.side_hydrophobic - s2.side_hydrophobic) };
    }

    class sync_data_ref {
    public:
        inline sync_data_ref(int8_t& back, int8_t& side_all,
            int8_t& side_polar, int8_t& side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

        inline auto& operator=(sync_data_cref const& other) {
            back = other.back;
            side_all = other.side_all;
            side_polar = other.side_polar;
            side_hydrophobic = other.side_hydrophobic;
            return *this;
        }

        inline auto& operator=(sync_data_ref const& other) {
            back = other.back;
            side_all = other.side_all;
            side_polar = other.side_polar;
            side_hydrophobic = other.side_hydrophobic;
            return *this;
        }

        inline auto& operator=(sync_data const& other) {
            back = other.back;
            side_all = other.side_all;
            side_polar = other.side_polar;
            side_hydrophobic = other.side_hydrophobic;
            return *this;
        }

        inline operator sync_data_cref() const {
            return { back, side_all, side_polar, side_hydrophobic };
        }

        inline operator sync_data() const {
            return { back, side_all, side_polar, side_hydrophobic };
        }

        inline auto& operator+=(sync_data_cref const& other) {
            back += other.back;
            side_all += other.side_all;
            side_polar += other.side_polar;
            side_hydrophobic += other.side_hydrophobic;
            return *this;
        }

        inline auto& operator-=(sync_data_cref const& other) {
            back -= other.back;
            side_all -= other.side_all;
            side_polar -= other.side_polar;
            side_hydrophobic -= other.side_hydrophobic;
            return *this;
        }

        inline auto& operator+=(sync_data const& other) {
            back += other.back;
            side_all += other.side_all;
            side_polar += other.side_polar;
            side_hydrophobic += other.side_hydrophobic;
            return *this;
        }

        inline auto& operator-=(sync_data const& other) {
            back -= other.back;
            side_all -= other.side_all;
            side_polar -= other.side_polar;
            side_hydrophobic -= other.side_hydrophobic;
            return *this;
        }

        inline auto& operator+=(sync_data_ref const& other) {
            back += other.back;
            side_all += other.side_all;
            side_polar += other.side_polar;
            side_hydrophobic += other.side_hydrophobic;
            return *this;
        }

        inline auto& operator-=(sync_data_ref const& other) {
            back -= other.back;
            side_all -= other.side_all;
            side_polar -= other.side_polar;
            side_hydrophobic -= other.side_hydrophobic;
            return *this;
        }

    public:
        int8_t &back, &side_all, &side_polar, &side_hydrophobic;
    };

    class sync_data_array {
    public:
        array<int8_t> back, side_all, side_polar, side_hydrophobic;

        inline sync_data_ref operator[](int idx) const {
            return { back[idx], side_all[idx], side_polar[idx],
                     side_hydrophobic[idx] };
        }
    };

    class sync_data_vector {
    public:
        vector<int8_t> back, side_all, side_polar, side_hydrophobic;
        int size_;

        explicit sync_data_vector(int n = 0);

        inline sync_data_ref operator[](int idx) {
            return { back[idx], side_all[idx], side_polar[idx],
                     side_hydrophobic[idx] };
        }

        inline sync_data_cref operator[](int idx) const {
            return { back[idx], side_all[idx], side_polar[idx],
                     side_hydrophobic[idx] };
        }

        inline sync_data_ref emplace_back() {
            ++size_;
            return { back.emplace_back(), side_all.emplace_back(),
                     side_polar.emplace_back(), side_hydrophobic.emplace_back()
            };
        }

        inline int size() const {
            return size_;
        }

        inline void clear() {
            back.clear();
            side_all.clear();
            side_polar.clear();
            side_hydrophobic.clear();
        }

        inline auto to_array() {
            sync_data_array a;
            a.back = back.to_array();
            a.side_all = side_all.to_array();
            a.side_polar = side_polar.to_array();
            a.side_hydrophobic = side_hydrophobic.to_array();
            return a;
        }
    };
}