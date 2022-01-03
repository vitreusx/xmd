

#pragma once
#include "meta.h"

template <typename T> struct const_ref_impl { using type = T const &; };

template <typename T> using const_ref = typename const_ref_impl<T>::type;

#define GEN_CONST_REF() GEN_CONST_REF_EXP(NAME(), FIELDS())

#define GEN_CONST_REF_EXP(name, fields) GEN_CONST_REF_(name, fields)

#define GEN_CONST_REF_(...) VFUNC(GEN_CONST_REF_, __VA_ARGS__)

#define GEN_CONST_REF_3(name, T1, x1)                                          \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{x1}                                                            \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_const_ref(name NO_SPEC() const &inst_)                              \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_5(name, T1, x1, T2, x2)                                  \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
        :                                                                      \
                                                                               \
          x1##_{inst_.x1()}                                                    \
                                                                               \
          ,                                                                    \
          x2##_{inst_.x2()}                                                    \
                                                                               \
          {};                                                                  \
                                                                               \
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_7(name, T1, x1, T2, x2, T3, x3)                          \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                  \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T4> x4                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          ,                                                                    \
          x4##_{ref_.x4()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
                                                                               \
    const_ref<T4> x4##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)         \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T4> x4                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T5> x5                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          ,                                                                    \
          x4##_{ref_.x4()}                                                     \
                                                                               \
          ,                                                                    \
          x5##_{ref_.x5()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
                                                                               \
    const_ref<T4> x4##_;                                                       \
                                                                               \
    const_ref<T5> x5##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6) \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T4> x4                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T5> x5                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T6> x6                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          ,                                                                    \
          x4##_{ref_.x4()}                                                     \
                                                                               \
          ,                                                                    \
          x5##_{ref_.x5()}                                                     \
                                                                               \
          ,                                                                    \
          x6##_{ref_.x6()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
                                                                               \
    const_ref<T4> x4##_;                                                       \
                                                                               \
    const_ref<T5> x5##_;                                                       \
                                                                               \
    const_ref<T6> x6##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, \
                         T7, x7)                                               \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T4> x4                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T5> x5                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T6> x6                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T7> x7                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          ,                                                                    \
          x4##_{ref_.x4()}                                                     \
                                                                               \
          ,                                                                    \
          x5##_{ref_.x5()}                                                     \
                                                                               \
          ,                                                                    \
          x6##_{ref_.x6()}                                                     \
                                                                               \
          ,                                                                    \
          x7##_{ref_.x7()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
                                                                               \
    inline const_ref<T7> x7() const { return x7##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
                                                                               \
    const_ref<T4> x4##_;                                                       \
                                                                               \
    const_ref<T5> x5##_;                                                       \
                                                                               \
    const_ref<T6> x6##_;                                                       \
                                                                               \
    const_ref<T7> x7##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };

#define GEN_CONST_REF_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, \
                         T7, x7, T8, x8)                                       \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name##_const_ref : public name##_expr<name##_const_ref NO_SPEC()> {    \
  public:                                                                      \
    name##_const_ref(                                                          \
                                                                               \
        const_ref<T1> x1                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T2> x2                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T3> x3                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T4> x4                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T5> x5                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T6> x6                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T7> x7                                                       \
                                                                               \
        ,                                                                      \
        const_ref<T8> x8                                                       \
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
    name##_const_ref(name NO_SPEC() const &inst_)                              \
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
    name##_const_ref(name##_ref NO_SPEC() const &ref_)                         \
        :                                                                      \
                                                                               \
          x1##_{ref_.x1()}                                                     \
                                                                               \
          ,                                                                    \
          x2##_{ref_.x2()}                                                     \
                                                                               \
          ,                                                                    \
          x3##_{ref_.x3()}                                                     \
                                                                               \
          ,                                                                    \
          x4##_{ref_.x4()}                                                     \
                                                                               \
          ,                                                                    \
          x5##_{ref_.x5()}                                                     \
                                                                               \
          ,                                                                    \
          x6##_{ref_.x6()}                                                     \
                                                                               \
          ,                                                                    \
          x7##_{ref_.x7()}                                                     \
                                                                               \
          ,                                                                    \
          x8##_{ref_.x8()}                                                     \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
                                                                               \
    inline const_ref<T7> x7() const { return x7##_; }                          \
                                                                               \
    inline const_ref<T8> x8() const { return x8##_; }                          \
                                                                               \
  protected:                                                                   \
    const_ref<T1> x1##_;                                                       \
                                                                               \
    const_ref<T2> x2##_;                                                       \
                                                                               \
    const_ref<T3> x3##_;                                                       \
                                                                               \
    const_ref<T4> x4##_;                                                       \
                                                                               \
    const_ref<T5> x5##_;                                                       \
                                                                               \
    const_ref<T6> x6##_;                                                       \
                                                                               \
    const_ref<T7> x7##_;                                                       \
                                                                               \
    const_ref<T8> x8##_;                                                       \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct const_ref_impl<NS_PREFIX() name NO_SPEC()> {                          \
    using type = NS_PREFIX() name##_const_ref NO_SPEC();                       \
  };
