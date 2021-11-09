#pragma once

namespace fastmd {
    template<typename U>
    ANY scalar3<U>::scalar3(U x, U y, U z):
        x{x}, y{y}, z{z} {}

    template<typename U>
    template<typename V>
    ANY scalar3<U> &scalar3<U>::operator=(const scalar3 <V> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator+=(const scalar3 <V> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator-=(const scalar3 <V> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator*=(const scalar3 <V> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator*=(const V &v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator/=(const scalar3 <V> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY scalar3 <U> &scalar3<U>::operator/=(const V &v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    template<typename U>
    template<typename V>
    ANY auto operator+(scalar3<U> const& u, scalar3<V> const& v) {
        return scalar3(u.x+v.x, u.y+v.y, u.z+v.z);
    }

    template<typename U>
    template<typename V>
    ANY auto operator-(scalar3<U> const& u, scalar3<V> const& v) {
        return scalar3(u.x-v.x, u.y-v.y, u.z-v.z);
    }

    template<typename U>
    ANY auto operator-(scalar3<U> const& u) {
        return scalar3(-u.x, -u.y, -u.z);
    }

    template<typename U>
    template<typename V>
    ANY auto operator*(scalar3<U> const& u, scalar3<V> const& v) {
        return scalar3(u.x*v.x, u.y*v.y, u.z*v.z);
    }

    template<typename U>
    template<typename V>
    ANY auto operator*(scalar3<U> const& u, V const& v) {
        return scalar3(u.x*v, u.y*v, u.z*v);
    }

    template<typename U>
    template<typename V>
    ANY auto operator*(U const& u, scalar3<V> const& v) {
        return scalar3(u*v.x, u*v.y, u*v.z);
    }

    template<typename U>
    template<typename V>
    ANY auto operator/(scalar3<U> const& u, scalar3<V> const& v) {
return scalar3(u.x/v.x, u.y/v.y, u.z/v.z);
    }

    template<typename U>
    template<typename V>
    ANY auto operator/(scalar3<U> const& u, V const& v) {
return scalar3(u.x/v, u.y/v, u.z/v);
    }

    template<typename U>
    template<typename V>
    ANY auto operator/(U const& u, scalar3<V> const& v) {
        return scalar3(u/v.x, u/v.y, u/v.z);
    }

    template<typename U>
    ANY auto norm(scalar3<U> const& u) {
#ifdef __CUDA_ARCH__
    if constexpr (std::is_same_v<U, float>) {
        return norm3df(u.x, u.y, u.z);
    }
    else if constexpr (std::is_same_v<U, double>) {
        return norm3d(u.x, u.y, u.z);
    }
    else {
        static_assert("cannot use norm with U different than float or double");
    }
#else
        return std::sqrt(norm_squared(u));
#endif
    }

    template<typename U>
    ANY auto norm_squared(scalar3<U> const& u) {
        return u.x*u.x+u.y*u.y+u.z*u.z;
    }

    template<typename U>
    ANY auto l1_norm(scalar3<U> const& u) {
        return std::abs(u.x)+std::abs(u.y)+std::abs(u.z);
    }

    template<typename U>
    ANY auto rnorm(scalar3<U> const& u) {
#ifdef __CUDA_ARCH__
        if constexpr (std::is_same_v<U, float>) {
            return rnorm3df(u.x, u.y, u.z);
        }
        else if constexpr (std::is_same_v<U, double>) {
            return rnorm3d(u.x, u.y, u.z);
        }
        else {
            return U(1) / norm(u);
        }
#else
        return U(1) / norm(u);
#endif
    }

    template<typename U>
    template<typename V>
    ANY auto dot(scalar3<U> const& u, scalar3<V> const& v) {
        return u.x*v.x+u.y*v.y+u.z*v.z;
    }

    template<typename U>
    template<typename V>
    ANY auto cross(scalar3<U> const& u, scalar3<V> const& v) {
        auto p = u.y*v.z-u.z*v.y;
        auto q = u.z*v.x-u.x*v.z;
        auto r = u.x*v.y-u.y*v.z;
        return scalar3(p, q, r);
    }
}