#pragma once
#include <cmath>
#include <cuda_runtime.h>

namespace fastmd {
    template<typename U>
    class vec3 {
    public:
        U x, y, z;

        __host__ __device__ vec3();

        __host__ __device__ vec3(U const& x, U const& y, U const& z);

        template<typename V>
        __host__ __device__ vec3<U>& operator=(vec3<V> const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator+=(vec3<V> const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator-=(vec3<V> const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator*=(vec3<V> const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator*=(V const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator/=(vec3<V> const& v);

        template<typename V>
        __host__ __device__ vec3<U>& operator/=(V const& v);
    };

    template<typename U, typename V>
    __host__ __device__ auto operator+(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator-(vec3<U> const& u, vec3<V> const& v);

    template<typename U>
    __host__ __device__ auto operator-(vec3<U> const& u);

    template<typename U, typename V>
    __host__ __device__ auto operator*(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator*(vec3<U> const& u, V const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator*(U const& u, vec3<V> const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator/(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator/(vec3<U> const& u, V const& v);

    template<typename U, typename V>
    __host__ __device__ auto operator/(U const& u, vec3<V> const& v);

    template<typename U>
    __host__ __device__ auto norm(vec3<U> const& u);

    template<typename U>
    __host__ __device__ auto norm_squared(vec3<U> const& u);

    template<typename U>
    __host__ __device__ auto l1_norm(vec3<U> const& u);

    template<typename U>
    __host__ __device__ auto rnorm(vec3<U> const& u);

    template<typename U, typename V>
    __host__ __device__ auto dot(vec3<U> const& u, vec3<V> const& v);

    template<typename U, typename V>
    __host__ __device__ auto cross(vec3<U> const& u, vec3<V> const& v);
}

#include "vec3.inl"
