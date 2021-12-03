#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>

namespace xmd::dynss {
    enum contact_status {
        FORMING_OR_FORMED, BREAKING
    };

    class contact_set {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }

        inline int extent() const {
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
                status.emplace_back();
                ref_time.emplace_back();

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
            status.clear();
            ref_time.clear();
            has_item_.clear();
            vacant_slots.clear();
        }

        vector<int> i1, i2;
        vector<contact_status> status;
        vector<float> ref_time;

    private:
        int extent_;
        vector<bool> has_item_;
        cyclic_buffer<int> vacant_slots;
    };
}