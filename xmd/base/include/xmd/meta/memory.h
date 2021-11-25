#pragma once
#include <xmd/meta/generics.h>

namespace xmd {
    MAKE_FUNCTOR(const_ref, T, T const&);
    MAKE_FUNCTOR(ref, T, T&);
    MAKE_FUNCTOR(ptr, T, T*);

    template<typename GenT, size_t... I>
    class gen_const_ref: public GenT::template liftFn<const_ref> {
    public:
        using Base = typename GenT::template liftFn<const_ref>;

        inline gen_const_ref(GenT const& x):
            Base(std::get<I>(x.fields())...) {};

        inline gen_const_ref(typename std::tuple_element_t<I, typename Base::field_types>... xs):
            Base(xs...) {};

        operator GenT() const {
            return { std::tuple_element_t<I, typename GenT::field_types>(std::get<I>(this->fields()))... };
        }
    };

    template<typename GenT, size_t... I>
    class gen_ref: public GenT::template liftFn<ref> {
    public:
        using Base = typename GenT::template liftFn<ref>;

        gen_ref(GenT& x):
            Base(std::get<I>(x.fields())...) {};

        gen_ref(typename std::tuple_element_t<I, typename Base::field_types>... xs):
            Base(xs...) {};

        template<typename X>
        auto& operator=(X const& x) {
            (..., ([&]() -> auto { std::get<I>(this->fields()) = std::get<I>(x.fields()); })());
            return *this;
        }

        operator GenT() const {
            return { std::tuple_element_t<I, typename GenT::field_types>(std::get<I>(this->fields()))... };
        }

        operator const_ref<GenT>() const {
            return { std::get<I>(this->fields())... };
        }

        ptr<GenT> operator&() {
            return { &std::get<I>(this->fields())... };
        }
    };

    template<typename GenT, size_t... I>
    class gen_ptr: public GenT::template liftFn<ptr> {
    public:
        using Base = typename GenT::template liftFn<ptr>;

        explicit gen_ptr(GenT* x):
            Base(&std::get<I>(x->fields())...) {};

        explicit gen_ptr(typename std::tuple_element_t<I, typename Base::field_types>... xs):
            Base(xs...) {};

        ref<GenT> operator*() {
            return { *std::get<I>(this->fields())... };
        }

        ref<GenT> operator[](size_t offset) {
            return { std::get<I>(this->fields())[offset]... };
        }

        gen_ptr operator+(std::ptrdiff_t offset) const {
            return { std::get<I>(this->fields())+offset... };
        }

        gen_ptr& operator+=(std::ptrdiff_t offset) {
            (..., ([&]() -> auto { std::get<I>(this->fields()) += offset; })());
            return *this;
        }

        gen_ptr operator-(std::ptrdiff_t offset) const {
            return *this + (-offset);
        }

        gen_ptr& operator-=(std::ptrdiff_t offset) {
            *this += (-offset);
            return *this;
        }
    };

    USE_GEN_IMPL(const_ref, gen_const_ref);
    USE_GEN_IMPL(ref, gen_ref);
    USE_GEN_IMPL(ptr, gen_ptr);
}