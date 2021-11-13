#pragma once
#include <cmath>

namespace fastmd {
    template<typename U>
    class vector3 {
    public:
        U x, y, z;

        inline vector3(U x, U y, U z);

        template<typename V>
        inline vector3<U>& operator=(vector3<V> const& v);

        template<typename V>
        inline vector3<U>& operator+=(vector3<V> const& v);

        template<typename V>
        inline vector3<U>& operator-=(vector3<V> const& v);

        template<typename V>
        inline vector3<U>& operator*=(vector3<V> const& v);

        template<typename V>
        inline vector3<U>& operator*=(V const& v);

        template<typename V>
        inline vector3<U>& operator/=(vector3<V> const& v);

        template<typename V>
        inline vector3<U>& operator/=(V const& v);
    };

    template<typename U, typename V>
    inline auto operator+(vector3<U> const& u, vector3<V> const& v);

    template<typename U, typename V>
    inline auto operator-(vector3<U> const& u, vector3<V> const& v);

    template<typename U>
    inline auto operator-(vector3<U> const& u);

    template<typename U, typename V>
    inline auto operator*(vector3<U> const& u, vector3<V> const& v);

    template<typename U, typename V>
    inline auto operator*(vector3<U> const& u, V const& v);

    template<typename U, typename V>
    inline auto operator*(U const& u, vector3<V> const& v);

    template<typename U, typename V>
    inline auto operator/(vector3<U> const& u, vector3<V> const& v);

    template<typename U, typename V>
    inline auto operator/(vector3<U> const& u, V const& v);

    template<typename U, typename V>
    inline auto operator/(U const& u, vector3<V> const& v);

    template<typename U>
    inline auto norm(vector3<U> const& u);

    template<typename U>
    inline auto norm_squared(vector3<U> const& u);

    template<typename U>
    inline auto l1_norm(vector3<U> const& u);

    template<typename U>
    inline auto rnorm(vector3<U> const& u);

    template<typename U, typename V>
    inline auto dot(vector3<U> const& u, vector3<V> const& v);

    template<typename U, typename V>
    inline auto cross(vector3<U> const& u, vector3<V> const& v);

    template<typename U>
    inline auto unit(vector3<U> const& u);

    using vector3f = vector3<float>;
    using vector3d = vector3<double>;
}

#include "detail/vector3.inl"
