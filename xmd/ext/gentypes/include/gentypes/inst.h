

#pragma once
#include "meta.h"

#define GEN_INST() GEN_INST_EXP(NAME(), FIELDS())

#define GEN_INST_EXP(name, fields) GEN_INST_(name, fields)

#define GEN_INST_(...) VFUNC(GEN_INST_, __VA_ARGS__)

#define GEN_INST_3(name, T1, x1)                                               \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_5(name, T1, x1, T2, x2)                                       \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
        :                                                                      \
                                                                               \
          x1##_{other.x1##_}                                                   \
                                                                               \
          ,                                                                    \
          x2##_{other.x2##_}                                                   \
                                                                               \
          {};                                                                  \
                                                                               \
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_7(name, T1, x1, T2, x2, T3, x3)                               \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                       \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3                                                           \
                                                                               \
        ,                                                                      \
        typename _T4>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        ,                                                                      \
        _T4 &&arg4                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          ,                                                                    \
          x4##_{std::forward<_T4>(arg4)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          ,                                                                    \
          x4##_{e.x4()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
    inline ref<T4> x4() { return x4##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
                                                                               \
    T4 x4##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)              \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3                                                           \
                                                                               \
        ,                                                                      \
        typename _T4                                                           \
                                                                               \
        ,                                                                      \
        typename _T5>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        ,                                                                      \
        _T4 &&arg4                                                             \
                                                                               \
        ,                                                                      \
        _T5 &&arg5                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          ,                                                                    \
          x4##_{std::forward<_T4>(arg4)}                                       \
                                                                               \
          ,                                                                    \
          x5##_{std::forward<_T5>(arg5)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          ,                                                                    \
          x4##_{e.x4()}                                                        \
                                                                               \
          ,                                                                    \
          x5##_{e.x5()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
    inline ref<T4> x4() { return x4##_; }                                      \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
    inline ref<T5> x5() { return x5##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
                                                                               \
    T4 x4##_;                                                                  \
                                                                               \
    T5 x5##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)      \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3                                                           \
                                                                               \
        ,                                                                      \
        typename _T4                                                           \
                                                                               \
        ,                                                                      \
        typename _T5                                                           \
                                                                               \
        ,                                                                      \
        typename _T6>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        ,                                                                      \
        _T4 &&arg4                                                             \
                                                                               \
        ,                                                                      \
        _T5 &&arg5                                                             \
                                                                               \
        ,                                                                      \
        _T6 &&arg6                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          ,                                                                    \
          x4##_{std::forward<_T4>(arg4)}                                       \
                                                                               \
          ,                                                                    \
          x5##_{std::forward<_T5>(arg5)}                                       \
                                                                               \
          ,                                                                    \
          x6##_{std::forward<_T6>(arg6)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          ,                                                                    \
          x4##_{e.x4()}                                                        \
                                                                               \
          ,                                                                    \
          x5##_{e.x5()}                                                        \
                                                                               \
          ,                                                                    \
          x6##_{e.x6()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
    inline ref<T4> x4() { return x4##_; }                                      \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
    inline ref<T5> x5() { return x5##_; }                                      \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
    inline ref<T6> x6() { return x6##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
                                                                               \
    T4 x4##_;                                                                  \
                                                                               \
    T5 x5##_;                                                                  \
                                                                               \
    T6 x6##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7)                                                        \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3                                                           \
                                                                               \
        ,                                                                      \
        typename _T4                                                           \
                                                                               \
        ,                                                                      \
        typename _T5                                                           \
                                                                               \
        ,                                                                      \
        typename _T6                                                           \
                                                                               \
        ,                                                                      \
        typename _T7>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        ,                                                                      \
        _T4 &&arg4                                                             \
                                                                               \
        ,                                                                      \
        _T5 &&arg5                                                             \
                                                                               \
        ,                                                                      \
        _T6 &&arg6                                                             \
                                                                               \
        ,                                                                      \
        _T7 &&arg7                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          ,                                                                    \
          x4##_{std::forward<_T4>(arg4)}                                       \
                                                                               \
          ,                                                                    \
          x5##_{std::forward<_T5>(arg5)}                                       \
                                                                               \
          ,                                                                    \
          x6##_{std::forward<_T6>(arg6)}                                       \
                                                                               \
          ,                                                                    \
          x7##_{std::forward<_T7>(arg7)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          ,                                                                    \
          x4##_{e.x4()}                                                        \
                                                                               \
          ,                                                                    \
          x5##_{e.x5()}                                                        \
                                                                               \
          ,                                                                    \
          x6##_{e.x6()}                                                        \
                                                                               \
          ,                                                                    \
          x7##_{e.x7()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
    inline ref<T4> x4() { return x4##_; }                                      \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
    inline ref<T5> x5() { return x5##_; }                                      \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
    inline ref<T6> x6() { return x6##_; }                                      \
                                                                               \
    inline const_ref<T7> x7() const { return x7##_; }                          \
    inline ref<T7> x7() { return x7##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
                                                                               \
    T4 x4##_;                                                                  \
                                                                               \
    T5 x5##_;                                                                  \
                                                                               \
    T6 x6##_;                                                                  \
                                                                               \
    T7 x7##_;                                                                  \
  };                                                                           \
  LEAVE_NS()

#define GEN_INST_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7, T8, x8)                                                \
                                                                               \
  ENTER_NS()                                                                   \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()> {                            \
  public:                                                                      \
    name(                                                                      \
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
    template <                                                                 \
                                                                               \
        typename _T1                                                           \
                                                                               \
        ,                                                                      \
        typename _T2                                                           \
                                                                               \
        ,                                                                      \
        typename _T3                                                           \
                                                                               \
        ,                                                                      \
        typename _T4                                                           \
                                                                               \
        ,                                                                      \
        typename _T5                                                           \
                                                                               \
        ,                                                                      \
        typename _T6                                                           \
                                                                               \
        ,                                                                      \
        typename _T7                                                           \
                                                                               \
        ,                                                                      \
        typename _T8>                                                          \
    name(                                                                      \
                                                                               \
        _T1 &&arg1                                                             \
                                                                               \
        ,                                                                      \
        _T2 &&arg2                                                             \
                                                                               \
        ,                                                                      \
        _T3 &&arg3                                                             \
                                                                               \
        ,                                                                      \
        _T4 &&arg4                                                             \
                                                                               \
        ,                                                                      \
        _T5 &&arg5                                                             \
                                                                               \
        ,                                                                      \
        _T6 &&arg6                                                             \
                                                                               \
        ,                                                                      \
        _T7 &&arg7                                                             \
                                                                               \
        ,                                                                      \
        _T8 &&arg8                                                             \
                                                                               \
        )                                                                      \
        :                                                                      \
                                                                               \
          x1##_{std::forward<_T1>(arg1)}                                       \
                                                                               \
          ,                                                                    \
          x2##_{std::forward<_T2>(arg2)}                                       \
                                                                               \
          ,                                                                    \
          x3##_{std::forward<_T3>(arg3)}                                       \
                                                                               \
          ,                                                                    \
          x4##_{std::forward<_T4>(arg4)}                                       \
                                                                               \
          ,                                                                    \
          x5##_{std::forward<_T5>(arg5)}                                       \
                                                                               \
          ,                                                                    \
          x6##_{std::forward<_T6>(arg6)}                                       \
                                                                               \
          ,                                                                    \
          x7##_{std::forward<_T7>(arg7)}                                       \
                                                                               \
          ,                                                                    \
          x8##_{std::forward<_T8>(arg8)}                                       \
                                                                               \
          {};                                                                  \
                                                                               \
    name(name const &other)                                                    \
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
    template <typename E>                                                      \
    name(name##_expr<E> const &e)                                              \
        :                                                                      \
                                                                               \
          x1##_{e.x1()}                                                        \
                                                                               \
          ,                                                                    \
          x2##_{e.x2()}                                                        \
                                                                               \
          ,                                                                    \
          x3##_{e.x3()}                                                        \
                                                                               \
          ,                                                                    \
          x4##_{e.x4()}                                                        \
                                                                               \
          ,                                                                    \
          x5##_{e.x5()}                                                        \
                                                                               \
          ,                                                                    \
          x6##_{e.x6()}                                                        \
                                                                               \
          ,                                                                    \
          x7##_{e.x7()}                                                        \
                                                                               \
          ,                                                                    \
          x8##_{e.x8()}                                                        \
                                                                               \
          {};                                                                  \
                                                                               \
    inline const_ref<T1> x1() const { return x1##_; }                          \
    inline ref<T1> x1() { return x1##_; }                                      \
                                                                               \
    inline const_ref<T2> x2() const { return x2##_; }                          \
    inline ref<T2> x2() { return x2##_; }                                      \
                                                                               \
    inline const_ref<T3> x3() const { return x3##_; }                          \
    inline ref<T3> x3() { return x3##_; }                                      \
                                                                               \
    inline const_ref<T4> x4() const { return x4##_; }                          \
    inline ref<T4> x4() { return x4##_; }                                      \
                                                                               \
    inline const_ref<T5> x5() const { return x5##_; }                          \
    inline ref<T5> x5() { return x5##_; }                                      \
                                                                               \
    inline const_ref<T6> x6() const { return x6##_; }                          \
    inline ref<T6> x6() { return x6##_; }                                      \
                                                                               \
    inline const_ref<T7> x7() const { return x7##_; }                          \
    inline ref<T7> x7() { return x7##_; }                                      \
                                                                               \
    inline const_ref<T8> x8() const { return x8##_; }                          \
    inline ref<T8> x8() { return x8##_; }                                      \
                                                                               \
  protected:                                                                   \
    T1 x1##_;                                                                  \
                                                                               \
    T2 x2##_;                                                                  \
                                                                               \
    T3 x3##_;                                                                  \
                                                                               \
    T4 x4##_;                                                                  \
                                                                               \
    T5 x5##_;                                                                  \
                                                                               \
    T6 x6##_;                                                                  \
                                                                               \
    T7 x7##_;                                                                  \
                                                                               \
    T8 x8##_;                                                                  \
  };                                                                           \
  LEAVE_NS()
