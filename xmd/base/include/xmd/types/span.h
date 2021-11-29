#pragma once

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

    private:
        T *data_;
        int size_;
    };
}