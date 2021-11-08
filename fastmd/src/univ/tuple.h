#pragma once
#include <tuple>
#include <type_traits>
#include "config.h"

namespace fastmd {
    template<typename... Ts>
    class tuple;

    template<>
    class tuple<> {
    public:
        static constexpr size_t size = 0;
    };

    template<typename T0, typename... TRest>
    class tuple<T0, TRest...> {
    public:
        static constexpr size_t size = 1 + sizeof...(TRest);

        ANY tuple() = default;
        ANY tuple(T0&& x0, TRest&&... xrest);

        template<typename U0, typename... URest>
        ANY tuple(tuple<U0, URest...> const& other);

        template<typename U0, typename... URest>
        ANY tuple& operator=(tuple<U0, URest...> const& other);

        T0 x0;
        tuple<TRest...> xrest;
    };

    template<size_t Idx, typename Tup>
    struct nth_impl;

    template<size_t Idx>
    struct nth_impl<Idx, tuple<>> {
        using type = void;
        static ANY void impl(tuple<>& t);
        static ANY void impl(tuple<> const& t);
    };

    template<typename T0, typename... TRest>
    struct nth_impl<0, tuple<T0, TRest...>> {
        using type = T0;
        static ANY type& impl(tuple<T0, TRest...>& t);
        static ANY type const& impl(tuple<T0, TRest...> const& t);
    };

    template<size_t Idx, typename T0, typename... TRest>
    struct nth_impl<Idx, tuple<T0, TRest...>> {
        using type = typename nth_impl<Idx-1, tuple<TRest...>>::type;
        static ANY type& impl(tuple<T0, TRest...>& t);
        static ANY type const& impl(tuple<T0, TRest...> const& t);
    };

    template<size_t Idx, typename Tup>
    using nth_type = typename nth_impl<Idx, Tup>::type;

    template<size_t Idx, typename Tup>
    ANY nth_type<Idx, Tup>& nth(Tup& t);

    template<size_t Idx, typename Tup>
    ANY nth_type<Idx, Tup> const& nth(Tup const& t);
}

#include "tuple.inl"
