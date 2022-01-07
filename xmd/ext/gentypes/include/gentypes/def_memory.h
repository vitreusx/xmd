#pragma once
#include <memory>

template <typename T>
struct uninitialized_fill_n
{
    static inline void impl(T *data, size_t n, T const &init)
    {
        std::uninitialized_fill_n(data, n, init);
    }
};

template <typename T>
struct uninitialized_copy_n
{
    static inline void impl(T const *from, size_t n, T *to)
    {
        std::uninitialized_copy_n(from, n, to);
    }
};

template <typename T>
struct uninitialized_move_n
{
    static inline void impl(T const *from, size_t n, T *to)
    {
        std::uninitialized_move_n(from, n, to);
    }
};

template <typename T>
struct destroy_n
{
    static inline void impl(T *data, size_t n)
    {
        std::destroy_n(data, n);
    }
};

template <typename T>
struct construct_at
{
    template <typename... Args>
    static inline void impl(T *p, Args &&...args)
    {
        ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
    }
};