#pragma once
#include "config.h"

namespace fastmd {
    template<typename T>
    struct no_allocator {
        using size_type = int;

        ANY T *allocate(size_type n) const;
        ANY void deallocate(T *ptr) const;
    };

    template<typename T>
    struct device_allocator {
        using size_type = int;

        ANY T *allocate(size_type n) const;
        ANY void deallocate(T *ptr) const;
    };

    template<typename T>
    struct host_allocator {
        ANY T *allocate(size_t n) const;
        ANY void deallocate(T *ptr) const;
    };
}

#include "alloc.inl"
