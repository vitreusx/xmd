#pragma once
#include <xmd/meta/generics.h>
#include <xmd/meta/span.h>
#include <xmd/meta/memory.h>
#include <vector>

namespace xmd {
    template<typename T>
    class def_vector {
    public:
        def_vector() = default;
        explicit def_vector(size_t n, T const& init = T()):
            data_(n, init) {};

        void push_back(T const& x) {
            data_.push_back(x);
        }

        void clear() {
            data_.clear();
        }

        T& operator[](size_t idx) {
            return data_[idx];
        }

        T const& operator[](size_t idx) const {
            return data_[idx];
        }

        T *data() {
            return data_.data();
        }

        size_t size() const {
            return data_.size();
        }

        span<T> span() {
            return { data(), (int)size() };
        }

        std::vector<T> data_;
    };

    MAKE_FUNCTOR(vector, T, def_vector<T>);

    template<typename GenT, size_t... I>
    class gen_vector: public GenT::template liftFn<vector> {
    public:
        using Base = typename GenT::template liftFn<vector>;

        gen_vector() = default;
        explicit gen_vector(size_t n, const_ref<GenT> init = GenT()):
            Base{std::tuple_element_t<I, typename Base::field_types>(n, std::get<I>(init.fields()))...},
            size_{n} {};

        void push_back(const_ref<GenT> x) {
            (..., std::get<I>(this->fields()).push_back(std::get<I>(x.fields())));
            ++size_;
        }

        void clear() {
            (..., std::get<I>(this->fields()).clear());
            size_ = 0;
        }

        ref<GenT> operator[](size_t idx) {
            return {std::get<I>(this->fields())[idx]... };
        }

        const_ref<GenT> operator[](size_t idx) const {
            return {std::get<I>(this->fields())[idx]... };
        }

        ptr<GenT> data() {
            return {std::get<I>(this->fields()).data()... };
        }

        size_t size() const {
            return size_;
        }

        span<GenT> span() {
            return { data(), (int)size() };
        }

    private:
        size_t size_ = 0;
    };

    USE_GEN_IMPL(vector, gen_vector);
}