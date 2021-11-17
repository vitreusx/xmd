#pragma once

namespace xmd {
    template<typename T>
    ref<T> array<T>::operator[](size_t idx) {
        return data_[idx];
    }

    template<typename T>
    const_ref<T> array<T>::operator[](size_t idx) const {
        return data_[idx];
    }

    template<typename T>
    size_t array<T>::size() const {
        return size_;
    }
}