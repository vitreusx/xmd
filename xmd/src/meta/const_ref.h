#pragma once
#include "meta/generics.h"

namespace xmd {
    MAKE_FUNCTOR(const_ref, T, T const&);

    template<typename GenT, size_t... I>
    class GEN_IMPL(const_ref): public GenT::template lift<FUNCTOR(const_ref)> {
    public:
        using super_t = typename GenT::template lift<FUNCTOR(const_ref)>;

        inline GEN_IMPL(const_ref)(GenT const& x):
            super_t(std::get<I>(x.fields())...) {};

        inline GEN_IMPL(const_ref)(typename std::tuple_element_t<I, typename super_t::field_types>... xs):
            super_t(xs...) {};

        operator GenT() const {
            return { std::get<I>(this->fields())... };
        }
    };

    USE_GEN_IMPL(const_ref);
}
