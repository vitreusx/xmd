#pragma once
#include <vector>
#include <xmd/types/cyclic_buffer.h>

namespace xmd::qa {
    class free_pair_list {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }

        int extent() const {
            return extent_;
        }

        int add() {
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

        void remove(int slot_idx) {
            has_item_[slot_idx] = false;
            vacant_slots.push(slot_idx);
        }

        void clear() {
            i1.clear();
            i2.clear();
        }

        std::vector<int> i1, i2;

    private:
        int extent_;

        std::vector<int8_t> has_item_;
        xmd::cyclic_buffer<int> vacant_slots;
    };
}