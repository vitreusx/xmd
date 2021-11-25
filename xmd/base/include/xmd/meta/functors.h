#pragma once

namespace xmd {
    struct identity {
        template<typename T>
        using type = T;
    };

    template<typename Functor1, typename Functor2>
    struct compose {
        template<typename T>
        using type = typename Functor1::template type<typename Functor2::template type<T>>;
    };

    template<typename Functor, typename T>
    using apply = typename Functor::template type<T>;

    template<template<typename> typename TypeFn>
    struct as_functor {
        template<typename T>
        using type = TypeFn<T>;
    };
}