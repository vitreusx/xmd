#pragma once

namespace xmd::nl {
    class neighbor_cell_list {
    public:
        int size() const {
            return size_;
        }

        int add() {
            cell_idx1.emplace_back();
            cell_idx1.emplace_back();
            return size_++;
        }

        void clear() {
            cell_idx1.clear();
            cell_idx1.clear();
            size_ = 0;
        }

        std::vector<int> cell_idx1, cell_idx2;

    private:
        int size_;
    };
}