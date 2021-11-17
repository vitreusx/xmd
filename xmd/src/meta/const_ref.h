#pragma once
#include "meta/generics.h"

namespace xmd {
    MAKE_FUNCTOR(const_ref, T const&);

    template<typename GenT, size_t... I>
    class GEN_IMPL(const_ref): public generic_tag {
    public:
        inline GEN_IMPL(const_ref)(GenT const& x):
            crefs(std::get<I>(x.fields())...) {};

    private:
        typename GenT::template lift<FUNCTOR(const_ref)> crefs;

    public:
        GENERIC_FROM_MEMBER(crefs);
    };

    USE_GEN_IMPL(const_ref);
}
