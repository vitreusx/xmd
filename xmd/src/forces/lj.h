#pragma once
#include "types/vec3.h"
#include "types/functors.h"

namespace xmd {
    template<template<typename> typename field>
    class lj {
    public:
        field<float> depth, r_min;
    };
}

#include "detail/lj.inl"
