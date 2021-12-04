#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>
#include <xmd/nl/nl_data.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::qa {
    class free_pair_set {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }

        inline int size() const {
            return extent_;
        }

        inline int add() {
            if (!vacant_slots.empty()) {
                int slot_idx = vacant_slots.pop();
                has_item_[slot_idx] = true;
                return slot_idx;
            }
            else {
                i1.emplace_back();
                i2.emplace_back();

                has_item_.push_back(true);
                return extent_++;
            }
        }

        inline void remove(int slot_idx) {
            has_item_[slot_idx] = false;
            vacant_slots.push(slot_idx);
        }

        inline void clear() {
            i1.clear();
            i2.clear();
        }

        vector<int> i1, i2;

    private:
        int extent_;
        vector<bool> has_item_;
        cyclic_buffer<int> vacant_slots;
    };

    class update_free_pairs {
    public:
        real max_formation_min_dist;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl::nl_data *nl;
        free_pair_set *pairs;

    public:
        void operator()() const;
    };
}