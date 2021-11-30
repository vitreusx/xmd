#pragma once
#include <xmd/types/vector.h>

namespace xmd::nl {
    class neighbor_cell_list {
    public:
        inline int size() const {
            return size_;
        }

        inline int add() {
            cell_idx1.emplace_back();
            cell_idx2.emplace_back();
            return size_++;
        }

        inline void clear() {
            cell_idx1.clear();
            cell_idx2.clear();
            size_ = 0;
        }

        vector<int> cell_idx1, cell_idx2;
        int size_;
    };
}