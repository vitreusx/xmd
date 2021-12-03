#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>

namespace xmd::dynss {
    class candidate_list {
    public:
        inline int size() const {
            return size_;
        }

        inline int add() {
            i1.emplace_back();
            i2.emplace_back();
            return size_++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            size_ = 0;
        }

        vector<int> i1, i2;

    private:
        int size_;
    };
}