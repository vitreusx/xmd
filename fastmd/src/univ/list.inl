#pragma once
#include <thrust/copy.h>
#include <thrust/execution_policy.h>

namespace fastmd::detail {
    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>::joint_list_impl(Allocs const& allocs):
        allocs {allocs} {};

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>::joint_list_impl(size_type n,
        pointer data, const Allocs &allocs): _size{n}, capacity{n}, data{data},
        allocs{allocs} {};

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>::~joint_list_impl() {
        (..., safe_dealloc<ISeq>());
        _size = capacity = 0;
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    template<size_t I>
    ANY void joint_list_impl<Ts, Allocs, ISeq...>::safe_dealloc() {
        if (nth<I>(data)) nth<I>(allocs).deallocate(nth<I>(data));
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>::joint_list_impl(joint_list_impl const& other) {
        *this = other;
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>&
    joint_list_impl<Ts, Allocs, ISeq...>::operator=(joint_list_impl const& other) {
        allocs = other.allocs;
        _size = other._size;
        capacity = other.capacity;

        data = pointer(nth<ISeq>(allocs).allocate(_size)...);
        (..., other.safe_copy<ISeq>(data));

        return *this;
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>::joint_list_impl(joint_list_impl&& other) {
        *this = std::move(other);
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY joint_list_impl<Ts, Allocs, ISeq...>&
    joint_list_impl<Ts, Allocs, ISeq...>::operator=(joint_list_impl&& other) {
        allocs = other.allocs;
        _size = other._size;
        capacity = other.capacity;
        data = other.data;

        other.data = pointer((typename std::allocator_traits<nth_type<ISeq, Allocs>>::pointer)nullptr...);
        other._size = other.capacity = 0;

        return *this;
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY typename joint_list_impl<Ts, Allocs, ISeq...>::size_type
    joint_list_impl<Ts, Allocs, ISeq...>::size() const {
        return _size;
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY typename joint_list_impl<Ts, Allocs, ISeq...>::reference_type
    joint_list_impl<Ts, Allocs, ISeq...>::operator[](size_type idx) {
        return reference_type(nth<ISeq>(data)[idx]...);
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY typename joint_list_impl<Ts, Allocs, ISeq...>::const_reference_type
    joint_list_impl<Ts, Allocs, ISeq...>::operator[](size_type idx) const {
        return reference_type(nth<ISeq>(data)[idx]...);
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    template<size_t I>
    ANY void joint_list_impl<Ts, Allocs, ISeq...>::safe_copy(pointer to) {

        if (nth<I>(data)) {
#ifdef __CUDA_ARCH__
            thrust::copy_n(thrust::device, nth<I>(data), _size, nth<I>(to));
#else
            thrust::copy_n(thrust::host, nth<I>(data), _size, nth<I>(to));
#endif
        }
    }

    template<typename Ts, typename Allocs, size_t... ISeq>
    ANY typename joint_list_impl<Ts, Allocs, ISeq...>::reference_type
    joint_list_impl<Ts, Allocs, ISeq...>::append(const_reference_type const& x) {
        if (_size == capacity) {
            size_type new_capacity = 2*capacity+8;
            pointer new_data(nth<ISeq>(allocs).allocate(new_capacity)...);
            (..., safe_copy<ISeq>(new_data));

            capacity = new_capacity;
            (..., nth<ISeq>(allocs).deallocate(nth<ISeq>(data)));
            data = new_data;
        }

        return ((*this)[_size++] = x);
    }
}