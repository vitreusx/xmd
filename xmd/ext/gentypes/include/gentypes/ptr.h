

#pragma once
#include "meta.h"

template <typename T> struct ptr_impl { using type = T *; };

template <typename T> using ptr = typename ptr_impl<T>::type;

template <typename T> inline T *ref_to_ptr(T &ref_) { return &ref_; }

#define GEN_PTR() GEN_PTR_EXP(NAME(), FIELDS())

#define GEN_PTR_EXP(name, fields) GEN_PTR_(name, fields)

#define GEN_PTR_(...) VFUNC(GEN_PTR_, __VA_ARGS__)

#define GEN_PTR_3(name, T1, x1)                                                \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_5(name, T1, x1, T2, x2)                                        \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_7(name, T1, x1, T2, x2, T3, x3)                                \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                        \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ptr<T4> x4                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          ,                                                                    \
          x4##_ptr{x4}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x4##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          ,                                                                    \
          x4##_ptr{&ptr_->x4()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x4##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      x4##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      x4##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
                                                                               \
    ptr<T4> x4##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x4())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)               \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ptr<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ptr<T5> x5                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          ,                                                                    \
          x4##_ptr{x4}                                                         \
                                                                               \
          ,                                                                    \
          x5##_ptr{x5}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x4##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x5##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          ,                                                                    \
          x4##_ptr{&ptr_->x4()}                                                \
                                                                               \
          ,                                                                    \
          x5##_ptr{&ptr_->x5()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x4##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x5##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      x4##_ptr += off;                                                         \
                                                                               \
      x5##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      x4##_ptr -= off;                                                         \
                                                                               \
      x5##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
                                                                               \
    ptr<T4> x4##_ptr;                                                          \
                                                                               \
    ptr<T5> x5##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x4())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x5())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)       \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ptr<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ptr<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ptr<T6> x6                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          ,                                                                    \
          x4##_ptr{x4}                                                         \
                                                                               \
          ,                                                                    \
          x5##_ptr{x5}                                                         \
                                                                               \
          ,                                                                    \
          x6##_ptr{x6}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x4##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x5##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x6##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          ,                                                                    \
          x4##_ptr{&ptr_->x4()}                                                \
                                                                               \
          ,                                                                    \
          x5##_ptr{&ptr_->x5()}                                                \
                                                                               \
          ,                                                                    \
          x6##_ptr{&ptr_->x6()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x4##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x5##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x6##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      x4##_ptr += off;                                                         \
                                                                               \
      x5##_ptr += off;                                                         \
                                                                               \
      x6##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      x4##_ptr -= off;                                                         \
                                                                               \
      x5##_ptr -= off;                                                         \
                                                                               \
      x6##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
                                                                               \
    ptr<T4> x4##_ptr;                                                          \
                                                                               \
    ptr<T5> x5##_ptr;                                                          \
                                                                               \
    ptr<T6> x6##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x4())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x5())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x6())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7)                                                         \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ptr<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ptr<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ptr<T6> x6                                                             \
                                                                               \
        ,                                                                      \
        ptr<T7> x7                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          ,                                                                    \
          x4##_ptr{x4}                                                         \
                                                                               \
          ,                                                                    \
          x5##_ptr{x5}                                                         \
                                                                               \
          ,                                                                    \
          x6##_ptr{x6}                                                         \
                                                                               \
          ,                                                                    \
          x7##_ptr{x7}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x4##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x5##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x6##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x7##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          ,                                                                    \
          x4##_ptr{&ptr_->x4()}                                                \
                                                                               \
          ,                                                                    \
          x5##_ptr{&ptr_->x5()}                                                \
                                                                               \
          ,                                                                    \
          x6##_ptr{&ptr_->x6()}                                                \
                                                                               \
          ,                                                                    \
          x7##_ptr{&ptr_->x7()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x7##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x7##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x4##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x5##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x6##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x7##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x7##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      x4##_ptr += off;                                                         \
                                                                               \
      x5##_ptr += off;                                                         \
                                                                               \
      x6##_ptr += off;                                                         \
                                                                               \
      x7##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x7##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      x4##_ptr -= off;                                                         \
                                                                               \
      x5##_ptr -= off;                                                         \
                                                                               \
      x6##_ptr -= off;                                                         \
                                                                               \
      x7##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
                                                                               \
    ptr<T4> x4##_ptr;                                                          \
                                                                               \
    ptr<T5> x5##_ptr;                                                          \
                                                                               \
    ptr<T6> x6##_ptr;                                                          \
                                                                               \
    ptr<T7> x7##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x4())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x5())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x6())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x7())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };

#define GEN_PTR_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7, T8, x8)                                                 \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_ptr {                                                           \
  public:                                                                      \
    inline name##_ptr() : name##_ptr{nullptr} {};                              \
                                                                               \
    inline name##_ptr(                                                         \
                                                                               \
        ptr<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ptr<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ptr<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ptr<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ptr<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ptr<T6> x6                                                             \
                                                                               \
        ,                                                                      \
        ptr<T7> x7                                                             \
                                                                               \
        ,                                                                      \
        ptr<T8> x8                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_ptr{x1}                                                         \
                                                                               \
          ,                                                                    \
          x2##_ptr{x2}                                                         \
                                                                               \
          ,                                                                    \
          x3##_ptr{x3}                                                         \
                                                                               \
          ,                                                                    \
          x4##_ptr{x4}                                                         \
                                                                               \
          ,                                                                    \
          x5##_ptr{x5}                                                         \
                                                                               \
          ,                                                                    \
          x6##_ptr{x6}                                                         \
                                                                               \
          ,                                                                    \
          x7##_ptr{x7}                                                         \
                                                                               \
          ,                                                                    \
          x8##_ptr{x8}                                                         \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(std::nullptr_t)                                          \
        :                                                                      \
                                                                               \
          x1##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x2##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x3##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x4##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x5##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x6##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x7##_ptr{nullptr}                                                    \
                                                                               \
          ,                                                                    \
          x8##_ptr{nullptr}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ptr(name NO_SPEC() * ptr_)                                   \
        :                                                                      \
                                                                               \
          x1##_ptr{&ptr_->x1()}                                                \
                                                                               \
          ,                                                                    \
          x2##_ptr{&ptr_->x2()}                                                \
                                                                               \
          ,                                                                    \
          x3##_ptr{&ptr_->x3()}                                                \
                                                                               \
          ,                                                                    \
          x4##_ptr{&ptr_->x4()}                                                \
                                                                               \
          ,                                                                    \
          x5##_ptr{&ptr_->x5()}                                                \
                                                                               \
          ,                                                                    \
          x6##_ptr{&ptr_->x6()}                                                \
                                                                               \
          ,                                                                    \
          x7##_ptr{&ptr_->x7()}                                                \
                                                                               \
          ,                                                                    \
          x8##_ptr{&ptr_->x8()}                                                \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref NO_SPEC() operator*(int) {                               \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x7##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x8##_ptr                                                            \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() operator*(int) const {                   \
      return {                                                                 \
                                                                               \
          *x1##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x2##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x3##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x4##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x5##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x6##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x7##_ptr                                                            \
                                                                               \
          ,                                                                    \
          *x8##_ptr                                                            \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    template <typename Idx>                                                    \
    inline name##_ref NO_SPEC() operator[](Idx idx) const {                    \
      return {                                                                 \
                                                                               \
          x1##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x2##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x3##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x4##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x5##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x6##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x7##_ptr[idx]                                                        \
                                                                               \
          ,                                                                    \
          x8##_ptr[idx]                                                        \
                                                                               \
      };                                                                       \
    };                                                                         \
                                                                               \
    inline operator bool() const { return x1##_ptr; }                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator+(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x7##_ptr + off                                                       \
                                                                               \
          ,                                                                    \
          x8##_ptr + off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator+=(Offset off) {                      \
                                                                               \
      x1##_ptr += off;                                                         \
                                                                               \
      x2##_ptr += off;                                                         \
                                                                               \
      x3##_ptr += off;                                                         \
                                                                               \
      x4##_ptr += off;                                                         \
                                                                               \
      x5##_ptr += off;                                                         \
                                                                               \
      x6##_ptr += off;                                                         \
                                                                               \
      x7##_ptr += off;                                                         \
                                                                               \
      x8##_ptr += off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() operator-(Offset off) const {                  \
      return {                                                                 \
                                                                               \
          x1##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x2##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x3##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x4##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x5##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x6##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x7##_ptr - off                                                       \
                                                                               \
          ,                                                                    \
          x8##_ptr - off                                                       \
                                                                               \
      };                                                                       \
    }                                                                          \
                                                                               \
    template <typename Offset>                                                 \
    inline name##_ptr NO_SPEC() &operator-=(Offset off) {                      \
                                                                               \
      x1##_ptr -= off;                                                         \
                                                                               \
      x2##_ptr -= off;                                                         \
                                                                               \
      x3##_ptr -= off;                                                         \
                                                                               \
      x4##_ptr -= off;                                                         \
                                                                               \
      x5##_ptr -= off;                                                         \
                                                                               \
      x6##_ptr -= off;                                                         \
                                                                               \
      x7##_ptr -= off;                                                         \
                                                                               \
      x8##_ptr -= off;                                                         \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
  public:                                                                      \
    ptr<T1> x1##_ptr;                                                          \
                                                                               \
    ptr<T2> x2##_ptr;                                                          \
                                                                               \
    ptr<T3> x3##_ptr;                                                          \
                                                                               \
    ptr<T4> x4##_ptr;                                                          \
                                                                               \
    ptr<T5> x5##_ptr;                                                          \
                                                                               \
    ptr<T6> x6##_ptr;                                                          \
                                                                               \
    ptr<T7> x7##_ptr;                                                          \
                                                                               \
    ptr<T8> x8##_ptr;                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NO_TEMPLATE()                                                                \
  NS_PREFIX()                                                                  \
  name##_ptr NO_SPEC()                                                         \
      ref_to_ptr(NS_PREFIX() name##_ref NO_SPEC() const &ref_) {               \
    return {                                                                   \
                                                                               \
        ref_to_ptr(ref_.x1())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x2())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x3())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x4())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x5())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x6())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x7())                                                  \
                                                                               \
            ,                                                                  \
        ref_to_ptr(ref_.x8())                                                  \
                                                                               \
    };                                                                         \
  }                                                                            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ptr_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ptr NO_SPEC();                             \
  };
