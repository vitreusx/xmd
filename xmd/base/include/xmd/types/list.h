#pragma once
#include "cyclic_buffer.h"

namespace xmd {
    template<typename Container>
    class item_list {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }

        auto operator[](int const& idx) {
            return container[idx];
        }

        auto operator[](int const& idx) {
            return container[idx];
        }

        int extent() const {
            return container.size();
        }

        auto add(typename Container::value_type const& item) {
            if (!vacant_slots.empty()) {
                int slot_idx = vacant_slots.pop();
                has_item_[slot_idx] = true;
                return (container[slot_idx] = item);
            }
            else {
                has_item_.push_back(true);
                return container.emplace_back(std::forward<Args>(item)...);
            }
        }

        void remove(int slot_idx) {
            has_item_[slot_idx] = false;
            vacant_slots.push(slot_idx);
            container[slot_idx].~decltype(container[slot_idx])();
        }

    private:
        Container container;
        std::vector<int8_t> has_item_;
        xmd::cyclic_buffer<int> vacant_slots;
    };
}