

#pragma once
#include "meta.h"

#define GEN_EXPR() GEN_EXPR_EXP(NAME(), FIELDS())

#define GEN_EXPR_EXP(name, fields) GEN_EXPR_(name, fields)

#define GEN_EXPR_(...) VFUNC(GEN_EXPR_, __VA_ARGS__)

#define GEN_EXPR_3(name, T1, x1)                                               \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_5(name, T1, x1, T2, x2)                                       \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_7(name, T1, x1, T2, x2, T3, x3)                               \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                       \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
                                                                               \
    inline auto x4() const { return static_cast<Impl const &>(*this).x4(); }   \
    inline auto x4() { return static_cast<Impl &>(*this).x4(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)              \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
                                                                               \
    inline auto x4() const { return static_cast<Impl const &>(*this).x4(); }   \
    inline auto x4() { return static_cast<Impl &>(*this).x4(); }               \
                                                                               \
    inline auto x5() const { return static_cast<Impl const &>(*this).x5(); }   \
    inline auto x5() { return static_cast<Impl &>(*this).x5(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)      \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
                                                                               \
    inline auto x4() const { return static_cast<Impl const &>(*this).x4(); }   \
    inline auto x4() { return static_cast<Impl &>(*this).x4(); }               \
                                                                               \
    inline auto x5() const { return static_cast<Impl const &>(*this).x5(); }   \
    inline auto x5() { return static_cast<Impl &>(*this).x5(); }               \
                                                                               \
    inline auto x6() const { return static_cast<Impl const &>(*this).x6(); }   \
    inline auto x6() { return static_cast<Impl &>(*this).x6(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7)                                                        \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
                                                                               \
    inline auto x4() const { return static_cast<Impl const &>(*this).x4(); }   \
    inline auto x4() { return static_cast<Impl &>(*this).x4(); }               \
                                                                               \
    inline auto x5() const { return static_cast<Impl const &>(*this).x5(); }   \
    inline auto x5() { return static_cast<Impl &>(*this).x5(); }               \
                                                                               \
    inline auto x6() const { return static_cast<Impl const &>(*this).x6(); }   \
    inline auto x6() { return static_cast<Impl &>(*this).x6(); }               \
                                                                               \
    inline auto x7() const { return static_cast<Impl const &>(*this).x7(); }   \
    inline auto x7() { return static_cast<Impl &>(*this).x7(); }               \
  };                                                                           \
  LEAVE_NS()

#define GEN_EXPR_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7, T8, x8)                                                \
                                                                               \
  ENTER_NS()                                                                   \
  template <typename Impl> class name##_expr {                                 \
  public:                                                                      \
    inline auto x1() const { return static_cast<Impl const &>(*this).x1(); }   \
    inline auto x1() { return static_cast<Impl &>(*this).x1(); }               \
                                                                               \
    inline auto x2() const { return static_cast<Impl const &>(*this).x2(); }   \
    inline auto x2() { return static_cast<Impl &>(*this).x2(); }               \
                                                                               \
    inline auto x3() const { return static_cast<Impl const &>(*this).x3(); }   \
    inline auto x3() { return static_cast<Impl &>(*this).x3(); }               \
                                                                               \
    inline auto x4() const { return static_cast<Impl const &>(*this).x4(); }   \
    inline auto x4() { return static_cast<Impl &>(*this).x4(); }               \
                                                                               \
    inline auto x5() const { return static_cast<Impl const &>(*this).x5(); }   \
    inline auto x5() { return static_cast<Impl &>(*this).x5(); }               \
                                                                               \
    inline auto x6() const { return static_cast<Impl const &>(*this).x6(); }   \
    inline auto x6() { return static_cast<Impl &>(*this).x6(); }               \
                                                                               \
    inline auto x7() const { return static_cast<Impl const &>(*this).x7(); }   \
    inline auto x7() { return static_cast<Impl &>(*this).x7(); }               \
                                                                               \
    inline auto x8() const { return static_cast<Impl const &>(*this).x8(); }   \
    inline auto x8() { return static_cast<Impl &>(*this).x8(); }               \
  };                                                                           \
  LEAVE_NS()
