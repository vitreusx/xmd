#pragma once
#include "types/vec3.h"
#include "types/functors.h"

namespace xmd {
    template<template<typename> typename field>
    class lj {
    public:
        field<float> depth, r_min;
    };

    template<>
    class lj<xmd::identity> {
    public:
        void operator()(float rnorm, float *V, float *dV_dr);
    };

    template<>
    class array<lj> {
    public:

    };
}

#include "detail/lj.inl"
