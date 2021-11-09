#pragma once
#include <type_traits>
#include <memory>

namespace fastmd {
    template<typename T, typename Alloc>
    class vector {
    public:
        using value_type = typename std::allocator_traits<Alloc>::value_type;
        static_assert(std::is_same_v<value_type, T>, "value type must be T");
        using pointer = typename std::allocator_traits<Alloc>::pointer;
        using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
        using size_type = typename std::allocator_traits<Alloc>::size_type;
        using reference = value_type&;
        using const_reference = value_type const&;

        ANY vector();
        ANY explicit vector(Alloc const& alloc);
        ANY explicit vector(size_t n, T const& value = T(),
            Alloc const& alloc = Alloc());
        ANY explicit vector(size_t n, Alloc const& alloc = Alloc());
        ANY vector(vector const& other);
        ANY vector(vector const& other, Alloc const& alloc);
        ANY vector(vector&& other) noexcept;
        ANY vector(vector&& other, Alloc const& alloc) noexcept;

        ANY vector& operator=(vector const& other);
        ANY vector&& operator=(vector&& other) noexcept;

        ANY ~vector();

        ANY size_type size() const;
        ANY size_type capacity() const;
        ANY pointer data() const;
        ANY void reserve(size_type new_cap);

        ANY reference operator[](size_type idx);
        ANY const_reference operator[](size_type idx) const;

        ANY void push_back(T const& x);
        template<typename... Args>
        ANY reference emplace_back(Args&&... args);
        ANY void pop_back();
        ANY void resize(T const& fill = T());

    private:
        pointer _data;
        int _size, _capacity;
        Alloc alloc;
    };
}

#include "vector.inl"
