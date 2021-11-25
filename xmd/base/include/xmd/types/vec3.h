#pragma once
#include <xmd/meta/generics.h>

namespace xmd {
    template<typename U>
    class vec3: public generic_tag {
    public:
        U x, y, z;

        vec3() = default;

        vec3(U x, U y, U z):
            x{x}, y{y}, z{z} {};

    public:
        using field_types = std::tuple<U, U, U>;
        FIELDS(x, y, z);

        template<typename Functor>
        using lift = vec3<apply<Functor, U>>;

        USING_LIFTFN;

    public:
        template<typename V>
        auto& operator=(vec3<V> const& v);

        template<typename V>
        auto& operator+=(vec3<V> const& v);

        template<typename V>
        auto& operator-=(vec3<V> const& v);

        template<typename V>
        auto& operator*=(vec3<V> const& v);

        template<typename S>
        auto& operator*=(S const& s);

        template<typename V>
        auto& operator/=(vec3<V> const& v);

        template<typename S>
        auto& operator/=(S const& s);
    };

    template<typename U, typename V>
    auto operator+(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    auto operator-(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    auto operator-(vec3<U> const& u);

    template<typename U, typename V>
    auto operator*(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename S>
    auto operator*(vec3<U> const& u, S const& s);

    template<typename S, typename V>
    auto operator*(S const& s, vec3<V> const& v);

    template<typename U, typename V>
    auto operator/(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename S>
    auto operator/(vec3<U> const& u, S const& s);

    template<typename S, typename V>
    auto operator/(S const& s, vec3<V> const& v);

    template<typename U>
    auto norm(vec3<U> const& u);

    template<typename U>
    auto norm_squared(vec3<U> const& u);

    template<typename U>
    auto l1_norm(vec3<U> const& u);

    template<typename U>
    auto rnorm(vec3<U> const& u);

    template<typename U, typename V>
    auto dot(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    auto cross(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    auto unit(vec3<U> const& u);

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
}

#include "detail/vec3.inl"