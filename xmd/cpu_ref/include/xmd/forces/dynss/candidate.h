#pragma once
#include <vector>
#include <xmd/types/cyclic_buffer.h>

namespace xmd::dynss {
    class candidate_list {
    public:
        int size() const {
            return size_;
        }

        int add() {
            i1.emplace_back();
            i2.emplace_back();
            return size_++;
        }

        void clear() {
            i1.clear();
            i2.clear();
            size_ = 0;
        }

        std::vector<int> i1, i2;

    private:
        int size_;
    };
}