#pragma once
#include <cmath>
#include "config.h"

namespace fastmd {
    template<typename U>
    class vec3 {
    public:
        U x, y, z;

        ANY vec3(U x, U y, U z);

        template<typename V>
        ANY vec3<U>& operator=(vec3<V> const& v);

        template<typename V>
        ANY vec3<U>& operator+=(vec3<V> const& v);

        template<typename V>
        ANY vec3<U>& operator-=(vec3<V> const& v);

        template<typename V>
        ANY vec3<U>& operator*=(vec3<V> const& v);

        template<typename V>
        ANY vec3<U>& operator*=(V const& v);

        template<typename V>
        ANY vec3<U>& operator/=(vec3<V> const& v);

        template<typename V>
        ANY vec3<U>& operator/=(V const& v);
    };

    template<typename U, typename V>
    ANY auto operator+(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    ANY auto operator-(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    ANY auto operator-(vec3<U> const& u);

    template<typename U, typename V>
    ANY auto operator*(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    ANY auto operator*(vec3<U> const& u, V const& v);

    template<typename U, typename V>
    ANY auto operator*(U const& u, vec3<V> const& v);

    template<typename U, typename V>
    ANY auto operator/(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    ANY auto operator/(vec3<U> const& u, V const& v);

    template<typename U, typename V>
    ANY auto operator/(U const& u, vec3<V> const& v);

    template<typename U>
    ANY auto norm(vec3<U> const& u);

    template<typename U>
    ANY auto norm_squared(vec3<U> const& u);

    template<typename U>
    ANY auto l1_norm(vec3<U> const& u);

    template<typename U>
    ANY auto rnorm(vec3<U> const& u);

    template<typename U, typename V>
    ANY auto dot(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    ANY auto cross(vec3<U> const& u, vec3<V> const& v);
}

#include "vec3.inl"
