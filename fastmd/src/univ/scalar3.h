#pragma once
#include <cmath>
#include "config.h"

namespace fastmd {
    template<typename U>
    class scalar3 {
    public:
        U x, y, z;

        ANY scalar3(U x, U y, U z);

        template<typename V>
        ANY scalar3<U>& operator=(scalar3<V> const& v);

        template<typename U, typename V>
        ANY friend auto operator+(scalar3<U> const& u, scalar3<V> const& v);

        template<typename V>
        ANY scalar3<U>& operator+=(scalar3<V> const& v);

        template<typename V>
        ANY friend auto operator-(scalar3<U> const& u, scalar3<V> const& v);

        template<typename U>
        ANY friend auto operator-(scalar3<U> const& u);

        template<typename V>
        ANY scalar3<U>& operator-=(scalar3<V> const& v);

        template<typename V>
        ANY friend auto operator*(scalar3<U> const& u, scalar3<V> const& v);

        template<typename V>
        ANY friend auto operator*(scalar3<U> const& u, V const& v);

        template<typename V>
        ANY friend auto operator*(U const& u, scalar3<V> const& v);

        template<typename V>
        ANY scalar3<U>& operator*=(scalar3<V> const& v);

        template<typename V>
        ANY scalar3<U>& operator*=(V const& v);

        template<typename V>
        ANY friend auto operator/(scalar3<U> const& u, scalar3<V> const& v);

        template<typename V>
        ANY friend auto operator/(scalar3<U> const& u, V const& v);

        template<typename V>
        ANY friend auto operator/(U const& u, scalar3<V> const& v);

        template<typename V>
        ANY scalar3<U>& operator/=(scalar3<V> const& v);

        template<typename V>
        ANY scalar3<U>& operator/=(V const& v);

        template<typename U>
        ANY friend auto norm(scalar3<U> const& u);

        template<typename U>
        ANY friend auto norm_squared(scalar3<U> const& u);

        template<typename U>
        ANY friend auto l1_norm(scalar3<U> const& u);

        template<typename U>
        ANY friend auto rnorm(scalar3<U> const& u);

        template<typename U, typename V>
        ANY friend auto dot(scalar3<U> const& u, scalar3<V> const& v);

        template<typename U, typename V>
        ANY friend auto cross(scalar3<U> const& u, scalar3<V> const& v);
    };

    using float3 = scalar3<float>;
    using double3 = scalar3<double>;
}

#include "scalar3.inl"
