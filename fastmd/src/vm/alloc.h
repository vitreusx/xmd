#pragma once
#include "config.h"

namespace fastmd {
    template<typename T>
    struct cu_allocator {
        ANY T *allocate(int n) const;
        ANY void deallocate(T *ptr) const;
    };
}

#include "alloc.inl"
