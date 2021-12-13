#pragma once
#include <xmd/types/vector.h>
#include "contact_type.h"
#include "sync_data.h"

namespace xmd::qa {
    class candidate_list {
    public:
        vector<int> i1, i2, free_pair_idx;
        vector<contact_type> type;
        sync_data_vector sync_diff1, sync_diff2;

        inline int const& size() const {
            return size_;
        }

        inline int add() {
            i1.emplace_back();
            i2.emplace_back();
            free_pair_idx.emplace_back();
            type.emplace_back();
            sync_diff1.emplace_back();
            sync_diff2.emplace_back();
            return size_++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            free_pair_idx.clear();
            type.clear();
            sync_diff1.clear();
            sync_diff2.clear();
            size_ = 0;
        }

    private:
        int size_;
    };
}