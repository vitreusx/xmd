#pragma once
#include "array.h"

namespace xmd {
    template<typename T>
    class span {
    public:
        inline explicit span(T *data, int size):
            data_{data}, size_{size} {};

        int size() const {
            return size_;
        }

        T& operator[](int idx) const {
            return data_[idx];
        }

        array<T> to_array() const {
            return data_;
        }

    private:
        T *data_;
        int size_;
    };
}