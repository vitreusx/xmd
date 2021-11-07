#pragma once

#include <thrust/copy.h>
#include <thrust/universal_vector.h>
#include <thrust/host_vector.h>

namespace fastmd::detail {
    template<typename T, typename Alloc>
    ANY def_list_impl<T, Alloc>::def_list_impl() {
        _data = nullptr;
        _size = _capacity = 0;
        alloc = Alloc();
    }

    template<typename T, typename Alloc>
    ANY def_list_impl<T, Alloc>::def_list_impl(const Alloc &alloc) {
        _data = nullptr;
        _size = _capacity = 0;
        this->alloc = alloc;
    }

    template<typename T, typename Alloc>
    ANY def_list_impl<T, Alloc>::def_list_impl(pointer data, size_type n,
        Alloc const& alloc) {

        _data = data;
        _size = _capacity = n;
        this->alloc = alloc;
    }

    template<typename T, typename Alloc>
    ANY def_list_impl<T, Alloc>::~def_list_impl() {
        if (_data) {
            alloc.deallocate(_data);
            _data = nullptr;
            _size = _capacity = 0;
        }
    }

    template<typename T, typename Alloc>
    ANY typename def_list_impl<T, Alloc>::size_type
    def_list_impl<T, Alloc>::size() const {
        return _size;
    }

    template<typename T, typename Alloc>
    ANY typename def_list_impl<T, Alloc>::size_type
    def_list_impl<T, Alloc>::capacity() const {
        return _size;
    }

    template<typename T, typename Alloc>
    ANY typename def_list_impl<T, Alloc>::pointer
    def_list_impl<T, Alloc>::data() {
        return _data;
    }

    template<typename T, typename Alloc>
    ANY T &def_list_impl<T, Alloc>::operator[](size_type idx) {
        return _data[idx];
    }

    template<typename T, typename Alloc>
    ANY T &def_list_impl<T, Alloc>::push_back(const T &x) {
        if (_size == _capacity) {
            int new_capacity = 2 * _capacity + 8;
            T *new_data = alloc.allocate(new_capacity);
            thrust::copy(_data, _data + _size, new_data);
            _data = new_data;
            _capacity = new_capacity;
        }

        return (_data[(_size++)-1] = x);
    }
}