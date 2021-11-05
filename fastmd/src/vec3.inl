#pragma once

namespace fastmd {
    template<typename U>
    __host__ __device__ vec3<U>::vec3(): x{U(0)}, y{U(0)}, z{U(0)} {}

    template<typename U>
    __host__ __device__ vec3<U>::vec3(const U &x, const U &y, const U &z):
        x{x}, y{y}, z{z} {}

    template<typename U>
    template<typename V>
    __host__ __device__ vec3<U> &vec3<U>::operator=(const vec3 <V> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator+=(const vec3 <V> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator-=(const vec3 <V> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator*=(const vec3 <V> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator*=(const V &v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator/=(const vec3 <V> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    __host__ __device__ vec3 <U> &vec3<U>::operator/=(const V &v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    template<typename U, typename V>
    __host__ __device__ auto operator+(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x+v.x, u.y+v.y, u.z+v.z);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator-(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x-v.x, u.y-v.y, u.z-v.z);
    }

    template<typename U>
    __host__ __device__ auto operator-(vec3<U> const& u) {
        return vec3(-u.x, -u.y, -u.z);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator*(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x*v.x, u.y*v.y, u.z*v.z);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator*(vec3<U> const& u, V const& v) {
        return vec3(u.x*v, u.y*v, u.z*v);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator*(U const& u, vec3<V> const& v) {
        return vec3(u*v.x, u*v.y, u*v.z);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator/(vec3<U> const& u, vec3<V> const& v) {
return vec3(u.x/v.x, u.y/v.y, u.z/v.z);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator/(vec3<U> const& u, V const& v) {
return vec3(u.x/v, u.y/v, u.z/v);
    }

    template<typename U, typename V>
    __host__ __device__ auto operator/(U const& u, vec3<V> const& v) {
        return vec3(u/v.x, u/v.y, u/v.z);
    }

    template<typename U>
    __host__ __device__ auto norm(vec3<U> const& u) {
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
    __host__ __device__ auto norm_squared(vec3<U> const& u) {
        return u.x*u.x+u.y*u.y+u.z*u.z;
    }

    template<typename U>
    __host__ __device__ auto l1_norm(vec3<U> const& u) {
        return std::abs(u.x)+std::abs(u.y)+std::abs(u.z);
    }

    template<typename U>
    __host__ __device__ auto rnorm(vec3<U> const& u) {
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

    template<typename U, typename V>
    __host__ __device__ auto dot(vec3<U> const& u, vec3<V> const& v) {
        return u.x*v.x+u.y*v.y+u.z*v.z;
    }

    template<typename U, typename V>
    __host__ __device__ auto cross(vec3<U> const& u, vec3<V> const& v) {
        auto p = u.y*v.z-u.z*v.y;
        auto q = u.z*v.x-u.x*v.z;
        auto r = u.x*v.y-u.y*v.z;
        return vec3(p, q, r);
    }
}