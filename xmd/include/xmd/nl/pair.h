#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/types/scalar.h>

namespace xmd::nl {
    struct nl_pair_span {
        array<int> i1, i2;
        array<bool> is_native;
        int size;
    };

    class nl_pair_vector {
    public:
        vector<int> i1, i2;
        vector<bool> is_native;
        int size;

        int push_back() {
            i1.push_back();
            i2.push_back();
            is_native.push_back();
            return size++;
        }

        void clear() {
            i1.clear();
            i2.clear();
            is_native.clear();
            size = 0;
        }

        explicit nl_pair_vector(int n = 0);
        nl_pair_span to_span();
    };
}