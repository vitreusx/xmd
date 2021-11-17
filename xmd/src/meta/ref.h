#pragma once
#include "meta/generics.h"
#include "meta/const_ref.h"

namespace xmd {
    MAKE_FUNCTOR(ref, T&);

    template<typename U>
    inline void setter(ref<U> x, const_ref<U> v) {
        x = v;
    }

    template<typename GenT, size_t... I>
    class GEN_IMPL(ref): public generic_tag {
    public:
        inline GEN_IMPL(ref)(GenT& x):
            refs(std::get<I>(x.fields())...) {};

        inline GEN_IMPL(ref)& operator=(const_ref<GenT> const& x) {
            (..., setter<std::tuple_element_t<I, typename GenT::field_types>>(std::get<I>(refs.fields()), std::get<I>(x.fields())));
            return *this;
        }

    private:
        typename GenT::template lift<FUNCTOR(ref)> refs;

    public:
        GENERIC_FROM_MEMBER(refs);
    };

    USE_GEN_IMPL(ref);
}
