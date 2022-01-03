

#pragma once
#include "meta.h"
#include "span.h"

template <typename T, typename Idx> class const_span_def {
public:
  inline explicit const_span_def(T const *data_ = nullptr, Idx size_ = 0)
      : data_{data_}, size_{size_} {};

  inline const_span_def(span_def<T, Idx> const &span)
      : const_span_def{span.data(), span.size()} {};

  inline T const *data() const { return data_; }

  inline Idx size() const { return size_; }

  inline T const &operator[](Idx idx) const { return data_[idx]; }

  inline T const &at(Idx idx) const { return data_[idx]; }

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
  ENTER_NS()                                                                   \
  TEMPLATE(typename, Idx)                                                      \
  class name##_const_span {                                                    \
  public:                                                                      \
    inline explicit name##_const_span(name##_const_ptr NO_SPEC()               \
                                          data_ = nullptr,                     \
                                      Idx size_ = 0)                           \
        : data_{data_}, size_{size_} {};                                       \
                                                                               \
    inline name##_const_span(name##_span SPEC(typename, Idx) const &span_)     \
        : name##_const_span{span_.data(), span_.size()} {};                    \
                                                                               \
    inline name##_const_ptr NO_SPEC() data() const { return data_; }           \
                                                                               \
    inline Idx size() const { return size_; }                                  \
                                                                               \
    inline name##_const_ref NO_SPEC() operator[](Idx idx) const {              \
      return data_[idx];                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() at(Idx idx) const { return data_[idx]; } \
                                                                               \
  protected:                                                                   \
    name##_const_ptr NO_SPEC() data_;                                          \
    Idx size_;                                                                 \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  TEMPLATE(typename, Idx)                                                      \
  struct const_span_impl<NS_PREFIX() name NO_SPEC(), Idx> {                    \
    using type = NS_PREFIX() name##_const_span SPEC(typename, Idx);            \
  };
