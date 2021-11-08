#pragma once

namespace fastmd {
    template<typename T>
    ANY typename device_allocator<T>::value_type *
    device_allocator<T>::allocate(size_type n) const {
#ifdef __CUDA_ARCH__
        return (T *)malloc(n * sizeof(T));
#else
        T *ptr;
        cudaMalloc((void**)&ptr, n * sizeof(T));
        return ptr;
#endif
    }

    template<typename T>
    ANY void device_allocator<T>::deallocate(T *ptr) const {
#ifdef __CUDA_ARCH__
        free(ptr);
#else
        cudaFree((void*)ptr);
#endif
    }

    template<typename T>
    ANY typename host_allocator<T>::value_type *
    host_allocator<T>::allocate(size_t n) const {
#ifdef __CUDA_ARCH__
        static_assert("host allocator can only be used on host");
#else
        return (T *)malloc(n * sizeof(T));
#endif
    }

    template<typename T>
    ANY void host_allocator<T>::deallocate(T *ptr) const {
#ifdef __CUDA_ARCH__
        static_assert("host_allocator<T> can only be used on host");
#else
        free((void*)ptr);
#endif
    }
}