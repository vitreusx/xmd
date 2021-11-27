#pragma once
#include <vector>
#include <xmd/types/list.h>

namespace xmd {
    class free_pair {
    public:
        free_pair(int i1, int i2):
            i1{i1}, i2{i2} {};

    public:
        int i1, i2;
    };

    class free_pair_cref {
    public:
        free_pair_cref(int const& i1, int const& i2):
            i1{i1}, i2{i2} {};

    public:
        int const &i1, &i2;
    };

    class free_pair_ref {
    public:
        inline free_pair_ref(int& i1, int& i2):
            i1{i1}, i2{i2} {};

        inline auto& operator=(free_pair_cref const& other) {
            i1 = other.i1;
            i2 = other.i2;
            return *this;
        }

        inline operator free_pair_cref() const {
            return { i1, i2 };
        }

    public:
        int &i1, &i2;
    };

    class free_pair_container {
    public:
        free_pair_ref operator[](int const& idx) {
            return { i1, i2, idx };
        }

        free_pair_const_ref operator[](int const& idx) const {
            return { i1, i2, idx };
        }

        size_t size() const {
            return
        }

        free_pair_ref emplace_back() {
            auto& _i1 = i1.emplace_back();
            auto& _i2 = i2.emplace_back()
        }

        using value_type =

    private:
        std::vector<int> i1, i2;
    };

    class free_pair_list {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }



        int extent() const {
            return extent_;
        }

        free_pair_ref add() {
            if (!vacant_slots.empty()) {
                int slot_idx = vacant_slots.pop();
                has_item_[slot_idx] = true;

            }
        }

        void remove(free_pair_ref item);

    private:
        std::vector<int> i1, i2;

        std::vector<int8_t> has_item_;
        xmd::cyclic_buffer<int> vacant_slots;
        int extent_;
    };
}