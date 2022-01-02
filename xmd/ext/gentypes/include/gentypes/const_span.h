

#pragma once
#include "meta.h"

template <typename T, typename Idx> class const_span_def {
public:
  explicit const_span_def(T const *data_ = nullptr, Idx size_ = 0)
      : data_{data_}, size_{size_} {};

  T const *data() const { return data_; }

  Idx size() const { return size_; }

  T const &operator[](Idx idx) const { return data_[idx]; }

protected:
  T const *data_;
  Idx size_;
};

template <typename T, typename Idx> struct const_span_impl {
  using type = const_span_def<T, Idx>;
};

template <typename T, typename Idx = int>
using const_span = typename const_span_impl<T, Idx>::type;

#define GEN_CONST_SPAN() GEN_CONST_SPAN_EXP(NAME())

#define GEN_CONST_SPAN_EXP(name) GEN_CONST_SPAN_(name)

#define GEN_CONST_SPAN_(name)                                                  \
                                                                               \
  TEMPLATE(typename, Idx)                                                      \
  class name##_const_span {                                                    \
  public:                                                                      \
    explicit name##_const_span(name##_const_ptr NO_SPEC() data_ = nullptr,     \
                               Idx size_ = 0)                                  \
        : data_{data_}, size_{size_} {};                                       \
                                                                               \
    name##_const_ptr NO_SPEC() data() const { return data_; }                  \
                                                                               \
    Idx size() const { return size_; }                                         \
                                                                               \
    name##_const_ref NO_SPEC() operator[](Idx idx) const {                     \
      return data_[idx];                                                       \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    name##_const_ptr NO_SPEC() data_;                                          \
    Idx size_;                                                                 \
  };                                                                           \
                                                                               \
  TEMPLATE(typename, Idx)                                                      \
  struct const_span_impl<name NO_SPEC(), Idx> {                                \
    using type = name##_const_span SPEC(typename, Idx);                        \
  };