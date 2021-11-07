#pragma once

namespace fastmd {
    template<typename T>
    ANY T *cu_allocator<T>::allocate(int n) const {
#ifdef __CUDA_ARCH__
        return (T *)malloc(n * sizeof(T));
#else
        T *ptr;
        cudaMalloc((void**)&ptr, n * sizeof(T));
        return ptr;
#endif
    }

    template<typename T>
    ANY void cu_allocator<T>::deallocate(T *ptr) const {
#ifdef __CUDA_ARCH__
        free(ptr);
#else
        cudaFree((void*)ptr);
#endif
    }
}