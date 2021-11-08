#pragma once

#include "tuple.h"

namespace fastmd {
    template<typename T0, typename... TRest>
    ANY tuple<T0, TRest...>::tuple(T0&& x0, TRest&&... xrest):
        x0{std::forward<T0>(x0)}, xrest{std::forward<TRest>(xrest)...} {};

    template<typename T0, typename... TRest>
    template<typename U0, typename... URest>
    ANY tuple<T0, TRest...>::tuple(tuple<U0, URest...> const& other):
        x0{other.x0}, xrest{other.xrest} {};

    template<typename T0, typename... TRest>
    template<typename U0, typename... URest>
    ANY tuple<T0, TRest...>& tuple<T0, TRest...>::operator=(
        const tuple<U0, URest...> &other) {

        x0 = other.x0;
        xrest = other.xrest;
        return *this;
    }

    template<size_t Idx>
    ANY void nth_impl<Idx, tuple<>>::impl(tuple<> &t) {}

    template<size_t Idx>
    ANY void nth_impl<Idx, tuple<>>::impl(tuple<> const &t) {}

    template<typename T0, typename... TRest>
    ANY typename nth_impl<0, tuple<T0, TRest...>>::type&
    nth_impl<0, tuple<T0, TRest...>>::impl(tuple<T0, TRest...>& t) {
        return t.x0;
    }

    template<typename T0, typename... TRest>
    ANY typename nth_impl<0, tuple<T0, TRest...>>::type const&
    nth_impl<0, tuple<T0, TRest...>>::impl(tuple<T0, TRest...> const& t) {
        return t.x0;
    }

    template<size_t Idx, typename T0, typename... TRest>
    ANY typename nth_impl<Idx, tuple<T0, TRest...>>::type&
    nth_impl<Idx, tuple<T0, TRest...>>::impl(tuple<T0, TRest...>& t) {
        return nth<Idx-1>(t.xrest);
    }

    template<size_t Idx, typename T0, typename... TRest>
    ANY typename nth_impl<Idx, tuple<T0, TRest...>>::type const&
    nth_impl<Idx, tuple<T0, TRest...>>::impl(tuple<T0, TRest...> const& t) {
        return nth<Idx-1>(t.xrest);
    }

    template<size_t Idx, typename Tup>
    ANY nth_type<Idx, Tup>& nth(Tup& t) {
        return nth_impl<Idx, Tup>::impl(t);
    }

    template<size_t Idx, typename Tup>
    ANY nth_type<Idx, Tup> const& nth(Tup const& t) {
        return nth_impl<Idx, Tup>::impl(t);
    }
}