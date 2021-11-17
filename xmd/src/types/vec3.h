#pragma once
#include "meta/generics.h"

namespace xmd {
    template<typename U>
    class vec3: public generic_tag {
    public:
        U x, y, z;

        inline vec3() = default;
        inline vec3(U&& x, U&& y, U&& z);

        auto fields();
        auto fields() const;

        template<typename Functor>
        using lift = vec3<apply<Functor, U>>;

    public:
        template<typename V>
        inline auto& operator=(vec3<V> const& v);

        template<typename V>
        inline auto& operator+=(vec3<V> const& v);

        template<typename V>
        inline auto& operator-=(vec3<V> const& v);

        template<typename V>
        inline auto& operator*=(vec3<V> const& v);

        template<typename S>
        inline auto& operator*=(S const& s);

        template<typename V>
        inline auto& operator/=(vec3<V> const& v);

        template<typename S>
        inline auto& operator/=(S const& s);
    };

    template<typename U, typename V>
    inline auto operator+(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    inline auto operator-(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    inline auto operator-(vec3<U> const& u);

    template<typename U, typename V>
    inline auto operator*(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename S>
    inline auto operator*(vec3<U> const& u, S const& s);

    template<typename S, typename V>
    inline auto operator*(S const& s, vec3<V> const& v);

    template<typename U, typename V>
    inline auto operator/(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename S>
    inline auto operator/(vec3<U> const& u, S const& s);

    template<typename S, typename V>
    inline auto operator/(S const& s, vec3<V> const& v);

    template<typename U>
    inline auto norm(vec3<U> const& u);

    template<typename U>
    inline auto norm_squared(vec3<U> const& u);

    template<typename U>
    inline auto l1_norm(vec3<U> const& u);

    template<typename U>
    inline auto rnorm(vec3<U> const& u);

    template<typename U, typename V>
    inline auto dot(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    inline auto cross(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    inline auto unit(vec3<U> const& u);

    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
}

#include "detail/vec3.inl"