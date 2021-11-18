#pragma once
#include "meta/generics.h"
#include "meta/const_ref.h"

namespace xmd {
    MAKE_FUNCTOR(ref, T, T&);

    template<typename U>
    inline void setter(ref<U> x, const_ref<U> v) {
        x = v;
    }

    template<typename GenT, size_t... I>
    class GEN_IMPL(ref): public GenT::template lift<FUNCTOR(ref)> {
    public:
        using super_t = typename GenT::template lift<FUNCTOR(ref)>;

        inline GEN_IMPL(ref)(GenT& x):
            super_t(std::get<I>(x.fields())...) {};

        inline GEN_IMPL(ref)(typename std::tuple_element_t<I, typename super_t::field_types>... xs):
            super_t(xs...) {};

        inline operator const_ref<GenT>() const {
            return { std::get<I>(this->fields())... };
        }

        inline auto& operator=(GenT const& x) {
            (..., setter<std::tuple_element_t<I, typename GenT::field_types>>(
                std::get<I>(this->fields()), std::get<I>(x.fields())));
            return *this;
        }

        inline auto& operator=(GEN_IMPL(ref) const& x) {
            (..., setter<std::tuple_element_t<I, typename GenT::field_types>>(
                std::get<I>(this->fields()), std::get<I>(x.fields())));
            return *this;
        }

        inline auto& operator=(const_ref<GenT> const& x) {
            (..., setter<std::tuple_element_t<I, typename GenT::field_types>>(
                std::get<I>(this->fields()), std::get<I>(x.fields())));
            return *this;
        }
    };

    USE_GEN_IMPL(ref);
}
