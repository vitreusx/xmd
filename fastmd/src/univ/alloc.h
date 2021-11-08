#pragma once
#include "config.h"

namespace fastmd {
    template<typename T>
    struct no_allocator {
        using value_type = T;
        using size_type = int;

        ANY value_type *allocate(size_type n) const;
        ANY void deallocate(value_type *ptr) const;
    };

    template<typename T>
    struct device_allocator {
        using value_type = T;
        using size_type = int;

        ANY value_type *allocate(size_type n) const;
        ANY void deallocate(value_type *ptr) const;
    };

    template<typename T>
    struct host_allocator {
        using value_type = T;

        ANY value_type *allocate(size_t n) const;
        ANY void deallocate(value_type *ptr) const;
    };
}

#include "alloc.inl"
