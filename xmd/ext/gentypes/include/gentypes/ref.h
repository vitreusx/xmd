

#pragma once
#include "meta.h"

template <typename T> struct ref_impl { using type = T &; };

template <typename T> using ref = typename ref_impl<T>::type;

#define GEN_REF() GEN_REF_EXP(NAME(), FIELDS())

#define GEN_REF_EXP(name, fields) GEN_REF_(name, fields)

#define GEN_REF_(...) VFUNC(GEN_REF_, __VA_ARGS__)

#define GEN_REF_3(name, T1, x1)                                                \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_5(name, T1, x1, T2, x2)                                        \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_7(name, T1, x1, T2, x2, T3, x3)                                \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                        \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ref<T4> x4                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          ,                                                                    \
          x4##_{x4}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          ,                                                                    \
          x4##_{inst_.x4()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          ,                                                                    \
          x4##_{other.x4##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      x4##_ = other.x4##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      x4##_ = std::move(other.x4##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      x4##_ = e.x4();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
    inline ref<T4> x4() const { return x4##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
                                                                               \
    ref<T4> x4##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)               \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ref<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ref<T5> x5                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          ,                                                                    \
          x4##_{x4}                                                            \
                                                                               \
          ,                                                                    \
          x5##_{x5}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          ,                                                                    \
          x4##_{inst_.x4()}                                                    \
                                                                               \
          ,                                                                    \
          x5##_{inst_.x5()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          ,                                                                    \
          x4##_{other.x4##_}                                                   \
                                                                               \
          ,                                                                    \
          x5##_{other.x5##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      x4##_ = other.x4##_;                                                     \
                                                                               \
      x5##_ = other.x5##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      x4##_ = std::move(other.x4##_);                                          \
                                                                               \
      x5##_ = std::move(other.x5##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      x4##_ = e.x4();                                                          \
                                                                               \
      x5##_ = e.x5();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
    inline ref<T4> x4() const { return x4##_; }                                \
                                                                               \
    inline ref<T5> x5() const { return x5##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
                                                                               \
    ref<T4> x4##_;                                                             \
                                                                               \
    ref<T5> x5##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)       \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ref<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ref<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ref<T6> x6                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          ,                                                                    \
          x4##_{x4}                                                            \
                                                                               \
          ,                                                                    \
          x5##_{x5}                                                            \
                                                                               \
          ,                                                                    \
          x6##_{x6}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          ,                                                                    \
          x4##_{inst_.x4()}                                                    \
                                                                               \
          ,                                                                    \
          x5##_{inst_.x5()}                                                    \
                                                                               \
          ,                                                                    \
          x6##_{inst_.x6()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          ,                                                                    \
          x4##_{other.x4##_}                                                   \
                                                                               \
          ,                                                                    \
          x5##_{other.x5##_}                                                   \
                                                                               \
          ,                                                                    \
          x6##_{other.x6##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      x4##_ = other.x4##_;                                                     \
                                                                               \
      x5##_ = other.x5##_;                                                     \
                                                                               \
      x6##_ = other.x6##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      x4##_ = std::move(other.x4##_);                                          \
                                                                               \
      x5##_ = std::move(other.x5##_);                                          \
                                                                               \
      x6##_ = std::move(other.x6##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      x4##_ = e.x4();                                                          \
                                                                               \
      x5##_ = e.x5();                                                          \
                                                                               \
      x6##_ = e.x6();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
    inline ref<T4> x4() const { return x4##_; }                                \
                                                                               \
    inline ref<T5> x5() const { return x5##_; }                                \
                                                                               \
    inline ref<T6> x6() const { return x6##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
                                                                               \
    ref<T4> x4##_;                                                             \
                                                                               \
    ref<T5> x5##_;                                                             \
                                                                               \
    ref<T6> x6##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7)                                                         \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ref<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ref<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ref<T6> x6                                                             \
                                                                               \
        ,                                                                      \
        ref<T7> x7                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          ,                                                                    \
          x4##_{x4}                                                            \
                                                                               \
          ,                                                                    \
          x5##_{x5}                                                            \
                                                                               \
          ,                                                                    \
          x6##_{x6}                                                            \
                                                                               \
          ,                                                                    \
          x7##_{x7}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          ,                                                                    \
          x4##_{inst_.x4()}                                                    \
                                                                               \
          ,                                                                    \
          x5##_{inst_.x5()}                                                    \
                                                                               \
          ,                                                                    \
          x6##_{inst_.x6()}                                                    \
                                                                               \
          ,                                                                    \
          x7##_{inst_.x7()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          ,                                                                    \
          x4##_{other.x4##_}                                                   \
                                                                               \
          ,                                                                    \
          x5##_{other.x5##_}                                                   \
                                                                               \
          ,                                                                    \
          x6##_{other.x6##_}                                                   \
                                                                               \
          ,                                                                    \
          x7##_{other.x7##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      x4##_ = other.x4##_;                                                     \
                                                                               \
      x5##_ = other.x5##_;                                                     \
                                                                               \
      x6##_ = other.x6##_;                                                     \
                                                                               \
      x7##_ = other.x7##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      x4##_ = std::move(other.x4##_);                                          \
                                                                               \
      x5##_ = std::move(other.x5##_);                                          \
                                                                               \
      x6##_ = std::move(other.x6##_);                                          \
                                                                               \
      x7##_ = std::move(other.x7##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      x4##_ = e.x4();                                                          \
                                                                               \
      x5##_ = e.x5();                                                          \
                                                                               \
      x6##_ = e.x6();                                                          \
                                                                               \
      x7##_ = e.x7();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
    inline ref<T4> x4() const { return x4##_; }                                \
                                                                               \
    inline ref<T5> x5() const { return x5##_; }                                \
                                                                               \
    inline ref<T6> x6() const { return x6##_; }                                \
                                                                               \
    inline ref<T7> x7() const { return x7##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
                                                                               \
    ref<T4> x4##_;                                                             \
                                                                               \
    ref<T5> x5##_;                                                             \
                                                                               \
    ref<T6> x6##_;                                                             \
                                                                               \
    ref<T7> x7##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };

#define GEN_REF_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7, T8, x8)                                                 \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()> {                \
  public:                                                                      \
    name##_ref(                                                                \
                                                                               \
        ref<T1> x1                                                             \
                                                                               \
        ,                                                                      \
        ref<T2> x2                                                             \
                                                                               \
        ,                                                                      \
        ref<T3> x3                                                             \
                                                                               \
        ,                                                                      \
        ref<T4> x4                                                             \
                                                                               \
        ,                                                                      \
        ref<T5> x5                                                             \
                                                                               \
        ,                                                                      \
        ref<T6> x6                                                             \
                                                                               \
        ,                                                                      \
        ref<T7> x7                                                             \
                                                                               \
        ,                                                                      \
        ref<T8> x8                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          ,                                                                    \
          x2##_{x2}                                                            \
                                                                               \
          ,                                                                    \
          x3##_{x3}                                                            \
                                                                               \
          ,                                                                    \
          x4##_{x4}                                                            \
                                                                               \
          ,                                                                    \
          x5##_{x5}                                                            \
                                                                               \
          ,                                                                    \
          x6##_{x6}                                                            \
                                                                               \
          ,                                                                    \
          x7##_{x7}                                                            \
                                                                               \
          ,                                                                    \
          x8##_{x8}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name NO_SPEC() & inst_)                                         \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          ,                                                                    \
          x3##_{inst_.x3()}                                                    \
                                                                               \
          ,                                                                    \
          x4##_{inst_.x4()}                                                    \
                                                                               \
          ,                                                                    \
          x5##_{inst_.x5()}                                                    \
                                                                               \
          ,                                                                    \
          x6##_{inst_.x6()}                                                    \
                                                                               \
          ,                                                                    \
          x7##_{inst_.x7()}                                                    \
                                                                               \
          ,                                                                    \
          x8##_{inst_.x8()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_ref(name##_ref NO_SPEC() const &other)                              \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          ,                                                                    \
          x3##_{other.x3##_}                                                   \
                                                                               \
          ,                                                                    \
          x4##_{other.x4##_}                                                   \
                                                                               \
          ,                                                                    \
          x5##_{other.x5##_}                                                   \
                                                                               \
          ,                                                                    \
          x6##_{other.x6##_}                                                   \
                                                                               \
          ,                                                                    \
          x7##_{other.x7##_}                                                   \
                                                                               \
          ,                                                                    \
          x8##_{other.x8##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {          \
                                                                               \
      x1##_ = other.x1##_;                                                     \
                                                                               \
      x2##_ = other.x2##_;                                                     \
                                                                               \
      x3##_ = other.x3##_;                                                     \
                                                                               \
      x4##_ = other.x4##_;                                                     \
                                                                               \
      x5##_ = other.x5##_;                                                     \
                                                                               \
      x6##_ = other.x6##_;                                                     \
                                                                               \
      x7##_ = other.x7##_;                                                     \
                                                                               \
      x8##_ = other.x8##_;                                                     \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_ref &operator=(name##_ref NO_SPEC() && other) {              \
                                                                               \
      x1##_ = std::move(other.x1##_);                                          \
                                                                               \
      x2##_ = std::move(other.x2##_);                                          \
                                                                               \
      x3##_ = std::move(other.x3##_);                                          \
                                                                               \
      x4##_ = std::move(other.x4##_);                                          \
                                                                               \
      x5##_ = std::move(other.x5##_);                                          \
                                                                               \
      x6##_ = std::move(other.x6##_);                                          \
                                                                               \
      x7##_ = std::move(other.x7##_);                                          \
                                                                               \
      x8##_ = std::move(other.x8##_);                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    template <typename E> name##_ref &operator=(name##_expr<E> const &e) {     \
                                                                               \
      x1##_ = e.x1();                                                          \
                                                                               \
      x2##_ = e.x2();                                                          \
                                                                               \
      x3##_ = e.x3();                                                          \
                                                                               \
      x4##_ = e.x4();                                                          \
                                                                               \
      x5##_ = e.x5();                                                          \
                                                                               \
      x6##_ = e.x6();                                                          \
                                                                               \
      x7##_ = e.x7();                                                          \
                                                                               \
      x8##_ = e.x8();                                                          \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline ref<T1> x1() const { return x1##_; }                                \
                                                                               \
    inline ref<T2> x2() const { return x2##_; }                                \
                                                                               \
    inline ref<T3> x3() const { return x3##_; }                                \
                                                                               \
    inline ref<T4> x4() const { return x4##_; }                                \
                                                                               \
    inline ref<T5> x5() const { return x5##_; }                                \
                                                                               \
    inline ref<T6> x6() const { return x6##_; }                                \
                                                                               \
    inline ref<T7> x7() const { return x7##_; }                                \
                                                                               \
    inline ref<T8> x8() const { return x8##_; }                                \
                                                                               \
  protected:                                                                   \
    ref<T1> x1##_;                                                             \
                                                                               \
    ref<T2> x2##_;                                                             \
                                                                               \
    ref<T3> x3##_;                                                             \
                                                                               \
    ref<T4> x4##_;                                                             \
                                                                               \
    ref<T5> x5##_;                                                             \
                                                                               \
    ref<T6> x6##_;                                                             \
                                                                               \
    ref<T7> x7##_;                                                             \
                                                                               \
    ref<T8> x8##_;                                                             \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<name NO_SPEC()> {                                            \
    using type = name##_ref NO_SPEC();                                         \
  };
