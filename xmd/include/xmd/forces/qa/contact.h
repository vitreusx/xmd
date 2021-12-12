#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>
#include "contact_type.h"
#include "sync_data.h"

namespace xmd::qa {
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
                type.emplace_back();
                status.emplace_back();
                ref_time.emplace_back();
                sync_diff1.emplace_back();
                sync_diff2.emplace_back();

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
            type.clear();
            status.clear();
            ref_time.clear();
            sync_diff1.clear();
            sync_diff2.clear();
            has_item_.clear();
            vacant_slots.clear();
            extent_ = 0;
        }

        vector<int> i1, i2;
        vector<contact_type> type;
        vector<contact_status> status;
        vector<real> ref_time;
        sync_data_vector sync_diff1, sync_diff2;

    private:
        int extent_;
        vector<bool> has_item_;
        cyclic_buffer<int> vacant_slots;
    };
}