

#pragma once
#include "meta.h"

template <typename T, typename Idx> class span_def {
public:
  explicit span_def(T *data_ = nullptr, Idx size_ = 0)
      : data_{data_}, size_{size_} {};

  T *data() const { return data_; }

  Idx size() const { return size_; }

  T &operator[](Idx idx) const { return data_[idx]; }

protected:
  T *data_;
  Idx size_;
};

template <typename T, typename Idx> struct span_impl {
  using type = span_def<T, Idx>;
};

template <typename T, typename Idx = int>
using span = typename span_impl<T, Idx>::type;

#define GEN_SPAN() GEN_SPAN_EXP(NAME())

#define GEN_SPAN_EXP(name) GEN_SPAN_(name)

#define GEN_SPAN_(name)                                                        \
                                                                               \
  ENTER_NS()                                                                   \
  TEMPLATE(typename, Idx)                                                      \
  class name##_span {                                                          \
  public:                                                                      \
    explicit name##_span(name##_ptr NO_SPEC() data_ = nullptr, Idx size_ = 0)  \
        : data_{data_}, size_{size_} {};                                       \
                                                                               \
    name##_ptr NO_SPEC() data() const { return data_; }                        \
                                                                               \
    Idx size() const { return size_; }                                         \
                                                                               \
    name##_ref NO_SPEC() operator[](Idx idx) const { return data_[idx]; }      \
                                                                               \
  protected:                                                                   \
    name##_ptr NO_SPEC() data_;                                                \
    Idx size_;                                                                 \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  TEMPLATE(typename, Idx)                                                      \
  struct span_impl<NS_PREFIX() name NO_SPEC(), Idx> {                          \
    using type = NS_PREFIX() name##_span SPEC(typename, Idx);                  \
  };
