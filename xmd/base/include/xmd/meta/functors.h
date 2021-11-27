#pragma once

namespace xmd {
    struct identity {
        template<typename T>
        using type = T;
    };

    template<typename F1, typename F2>
    struct compose {
        template<typename T>
        using type = typename F1::template type<typename F2::template type<T>>;
    };

    template<typename F, typename T>
    using apply = typename F::template type<T>;

    template<template<typename> typename Template>
    struct as_functor {
        template<typename T>
        using type = Template<T>;
    };
}