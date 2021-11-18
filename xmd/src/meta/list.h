#pragma once
#include <vector>
#include "meta/generics.h"
#include "meta/const_ref.h"
#include "meta/ref.h"

namespace xmd {
    template<typename T>
    class def_list {
    public:
        inline def_list() = default;
        inline def_list(size_t size, T const& x):
            data(size, x) {};

        inline ref<T> operator[](size_t idx) {
            return data[idx];
        }

        inline const_ref<T> operator[](size_t idx) const {
            return data[idx];
        }

        inline size_t size() const {
            return data.size();
        }

        inline void push_back(const_ref<T> const& x) {
            data.push_back(x);
        }

        inline void erase(size_t idx) {
            data.erase(data.begin() + idx);
        }

        inline void clear() {
            data.clear();
        }

    private:
        std::vector<T> data;
    };

    MAKE_FUNCTOR(list, T, def_list<T>);

    template<typename GenT, size_t... I>
    class GEN_IMPL(list): public GenT::template lift<FUNCTOR(list)> {
    public:
        using super_t = typename GenT::template lift<FUNCTOR(list)>;

        inline GEN_IMPL(list)() = default;
        inline GEN_IMPL(list)(size_t size, const_ref<GenT> init = GenT()):
            super_t{typename std::tuple_element_t<I, typename super_t::field_types>(
                size, std::get<I>(init.fields()))...},
            size_{size} {};

        inline ref<GenT> operator[](size_t idx) {
            return {std::get<I>(this->fields())[idx]...};
        }

        inline const_ref<GenT> operator[](size_t idx) const {
            return {std::get<I>(this->fields())[idx]...};
        }

        inline size_t size() const {
            return size_;
        }

        inline void push_back(const_ref<GenT> const& x) {
            (..., std::get<I>(this->fields()).push_back(std::get<I>(x.fields())));
            ++size_;
        }

        inline void erase(size_t idx) {
            (..., std::get<I>(this->fields()).erase(idx));
            --size_;
        }

        inline void clear() {
            (..., std::get<I>(this->fields()).clear());
            size_ = 0;
        }

    private:
        size_t size_;
    };

    USE_GEN_IMPL(list);
}