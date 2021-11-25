#pragma once
#include <xmd/meta/memory.h>

namespace xmd {
    template<typename T>
    class def_span {
    public:
        def_span(ptr<T> data, int size):
            data_{data}, size_{size} {};

        ptr<T> data() const {
            return data_;
        }

        int size() const {
            return size_;
        }

        ref<T> operator[](int idx) {
            return *(data_ + idx);
        }

        const_ref<T> operator[](int idx) const {
            return *(data_ + idx);
        }

    private:
        ptr<T> data_;
        int size_;
    };

    MAKE_FUNCTOR(span, T, def_span<T>);
}