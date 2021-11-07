#pragma once
#include "alloc.h"
#include <thrust/copy.h>

namespace fastmd::detail {
    template<typename T>
    ANY def_device_list_impl<T>::def_device_list_impl() {
        _data = nullptr;
        _size = _capacity = 0;
    }

    template<typename T>
    ANY def_device_list_impl<T>::def_device_list_impl(int n, const T &init) {
        _data = cu_allocator<T>().allocate(n);
        _size = _capacity = n;
        thrust::fill(_data, _data + n, init);
    }

    template<typename T>
    ANY def_device_list_impl<T>::~def_device_list_impl<T>() {
        if (_data) {
            cu_allocator<T>().deallocate(_data);
            _data = nullptr;
            _size = _capacity = 0;
        }
    }

    template<typename T>
    ANY int def_device_list_impl<T>::size() const {
        return _size;
    }

    template<typename T>
    ANY int def_device_list_impl<T>::capacity() const {
        return _size;
    }

    template<typename T>
    ANY T *def_device_list_impl<T>::data() {
        return _data;
    }

    template<typename T>
    ANY T& def_device_list_impl<T>::operator[](int idx) {
        return _data[idx];
    }

    template<typename T>
    ANY T& def_device_list_impl<T>::push_back(const T &x) {
        if (_size == _capacity) {
            int new_capacity = 2 * _capacity + 8;
            T *new_data = cu_allocator<T>().allocate(new_capacity);
            thrust::copy(_data, _data + _size, new_data);
            _data = new_data;
            _capacity = new_capacity;
        }
        return (_data[(_size++) - 1] = x);
    }
}