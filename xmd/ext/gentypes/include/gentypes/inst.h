

#pragma once
#include "def_memory.h"
#include "meta.h"
#include <boost/serialization/access.hpp>

#define INST_CTORS() INST_CTORS_EXP(NAME(), FIELDS())

#define INST_CTORS_EXP(name, fields) INST_CTORS_(name, fields)

#define INST_CTORS_(...) VFUNC_ALT2(INST_CTORS_, __VA_ARGS__)

#define INST_CTORS_3(name, T1, x1)                                             \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        {};

#define INST_CTORS_5(name, T1, x1, T2, x2)                                     \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        {};

#define INST_CTORS_7(name, T1, x1, T2, x2, T3, x3)                             \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        {};

#define INST_CTORS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                     \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      ,                                                                        \
      typename U4                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      ,                                                                        \
      U4 &&x4##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        ,                                                                      \
        x4##_{std::forward<U4>(x4##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        ,                                                                      \
        x4##_{other.x4##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        ,                                                                      \
        x4##_{e.x4()}                                                          \
                                                                               \
        {};

#define INST_CTORS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)            \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      ,                                                                        \
      typename U4                                                              \
                                                                               \
      ,                                                                        \
      typename U5                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      ,                                                                        \
      U4 &&x4##_init                                                           \
                                                                               \
      ,                                                                        \
      U5 &&x5##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        ,                                                                      \
        x4##_{std::forward<U4>(x4##_init)}                                     \
                                                                               \
        ,                                                                      \
        x5##_{std::forward<U5>(x5##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        ,                                                                      \
        x4##_{other.x4##_}                                                     \
                                                                               \
        ,                                                                      \
        x5##_{other.x5##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        ,                                                                      \
        x4##_{e.x4()}                                                          \
                                                                               \
        ,                                                                      \
        x5##_{e.x5()}                                                          \
                                                                               \
        {};

#define INST_CTORS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)    \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      ,                                                                        \
      typename U4                                                              \
                                                                               \
      ,                                                                        \
      typename U5                                                              \
                                                                               \
      ,                                                                        \
      typename U6                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      ,                                                                        \
      U4 &&x4##_init                                                           \
                                                                               \
      ,                                                                        \
      U5 &&x5##_init                                                           \
                                                                               \
      ,                                                                        \
      U6 &&x6##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        ,                                                                      \
        x4##_{std::forward<U4>(x4##_init)}                                     \
                                                                               \
        ,                                                                      \
        x5##_{std::forward<U5>(x5##_init)}                                     \
                                                                               \
        ,                                                                      \
        x6##_{std::forward<U6>(x6##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        ,                                                                      \
        x4##_{other.x4##_}                                                     \
                                                                               \
        ,                                                                      \
        x5##_{other.x5##_}                                                     \
                                                                               \
        ,                                                                      \
        x6##_{other.x6##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        ,                                                                      \
        x4##_{e.x4()}                                                          \
                                                                               \
        ,                                                                      \
        x5##_{e.x5()}                                                          \
                                                                               \
        ,                                                                      \
        x6##_{e.x6()}                                                          \
                                                                               \
        {};

#define INST_CTORS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7)                                                  \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      ,                                                                        \
      typename U4                                                              \
                                                                               \
      ,                                                                        \
      typename U5                                                              \
                                                                               \
      ,                                                                        \
      typename U6                                                              \
                                                                               \
      ,                                                                        \
      typename U7                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      ,                                                                        \
      U4 &&x4##_init                                                           \
                                                                               \
      ,                                                                        \
      U5 &&x5##_init                                                           \
                                                                               \
      ,                                                                        \
      U6 &&x6##_init                                                           \
                                                                               \
      ,                                                                        \
      U7 &&x7##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        ,                                                                      \
        x4##_{std::forward<U4>(x4##_init)}                                     \
                                                                               \
        ,                                                                      \
        x5##_{std::forward<U5>(x5##_init)}                                     \
                                                                               \
        ,                                                                      \
        x6##_{std::forward<U6>(x6##_init)}                                     \
                                                                               \
        ,                                                                      \
        x7##_{std::forward<U7>(x7##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        ,                                                                      \
        x4##_{other.x4##_}                                                     \
                                                                               \
        ,                                                                      \
        x5##_{other.x5##_}                                                     \
                                                                               \
        ,                                                                      \
        x6##_{other.x6##_}                                                     \
                                                                               \
        ,                                                                      \
        x7##_{other.x7##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        ,                                                                      \
        x4##_{e.x4()}                                                          \
                                                                               \
        ,                                                                      \
        x5##_{e.x5()}                                                          \
                                                                               \
        ,                                                                      \
        x6##_{e.x6()}                                                          \
                                                                               \
        ,                                                                      \
        x7##_{e.x7()}                                                          \
                                                                               \
        {};

#define INST_CTORS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7, T8, x8)                                          \
                                                                               \
  template <                                                                   \
                                                                               \
      typename U1                                                              \
                                                                               \
      ,                                                                        \
      typename U2                                                              \
                                                                               \
      ,                                                                        \
      typename U3                                                              \
                                                                               \
      ,                                                                        \
      typename U4                                                              \
                                                                               \
      ,                                                                        \
      typename U5                                                              \
                                                                               \
      ,                                                                        \
      typename U6                                                              \
                                                                               \
      ,                                                                        \
      typename U7                                                              \
                                                                               \
      ,                                                                        \
      typename U8                                                              \
                                                                               \
      >                                                                        \
  inline name(                                                                 \
                                                                               \
      U1 &&x1##_init                                                           \
                                                                               \
      ,                                                                        \
      U2 &&x2##_init                                                           \
                                                                               \
      ,                                                                        \
      U3 &&x3##_init                                                           \
                                                                               \
      ,                                                                        \
      U4 &&x4##_init                                                           \
                                                                               \
      ,                                                                        \
      U5 &&x5##_init                                                           \
                                                                               \
      ,                                                                        \
      U6 &&x6##_init                                                           \
                                                                               \
      ,                                                                        \
      U7 &&x7##_init                                                           \
                                                                               \
      ,                                                                        \
      U8 &&x8##_init                                                           \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{std::forward<U1>(x1##_init)}                                     \
                                                                               \
        ,                                                                      \
        x2##_{std::forward<U2>(x2##_init)}                                     \
                                                                               \
        ,                                                                      \
        x3##_{std::forward<U3>(x3##_init)}                                     \
                                                                               \
        ,                                                                      \
        x4##_{std::forward<U4>(x4##_init)}                                     \
                                                                               \
        ,                                                                      \
        x5##_{std::forward<U5>(x5##_init)}                                     \
                                                                               \
        ,                                                                      \
        x6##_{std::forward<U6>(x6##_init)}                                     \
                                                                               \
        ,                                                                      \
        x7##_{std::forward<U7>(x7##_init)}                                     \
                                                                               \
        ,                                                                      \
        x8##_{std::forward<U8>(x8##_init)}                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name(name const &other)                                               \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        ,                                                                      \
        x3##_{other.x3##_}                                                     \
                                                                               \
        ,                                                                      \
        x4##_{other.x4##_}                                                     \
                                                                               \
        ,                                                                      \
        x5##_{other.x5##_}                                                     \
                                                                               \
        ,                                                                      \
        x6##_{other.x6##_}                                                     \
                                                                               \
        ,                                                                      \
        x7##_{other.x7##_}                                                     \
                                                                               \
        ,                                                                      \
        x8##_{other.x8##_}                                                     \
                                                                               \
        {};                                                                    \
                                                                               \
  template <typename E>                                                        \
  inline name(name##_expr<E> const &e)                                         \
      :                                                                        \
                                                                               \
        x1##_{e.x1()}                                                          \
                                                                               \
        ,                                                                      \
        x2##_{e.x2()}                                                          \
                                                                               \
        ,                                                                      \
        x3##_{e.x3()}                                                          \
                                                                               \
        ,                                                                      \
        x4##_{e.x4()}                                                          \
                                                                               \
        ,                                                                      \
        x5##_{e.x5()}                                                          \
                                                                               \
        ,                                                                      \
        x6##_{e.x6()}                                                          \
                                                                               \
        ,                                                                      \
        x7##_{e.x7()}                                                          \
                                                                               \
        ,                                                                      \
        x8##_{e.x8()}                                                          \
                                                                               \
        {};

#define INST_ASSIGN_COPY() INST_ASSIGN_COPY_EXP(NAME(), FIELDS())

#define INST_ASSIGN_COPY_EXP(name, fields) INST_ASSIGN_COPY_(name, fields)

#define INST_ASSIGN_COPY_(...) VFUNC_ALT2(INST_ASSIGN_COPY_, __VA_ARGS__)

#define INST_ASSIGN_COPY_3(name, T1, x1)                                       \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_5(name, T1, x1, T2, x2)                               \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_7(name, T1, x1, T2, x2, T3, x3)                       \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_9(name, T1, x1, T2, x2, T3, x3, T4, x4)               \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    x4##_ = other.x4##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)      \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    x4##_ = other.x4##_;                                                       \
                                                                               \
    x5##_ = other.x5##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6)                                                \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    x4##_ = other.x4##_;                                                       \
                                                                               \
    x5##_ = other.x5##_;                                                       \
                                                                               \
    x6##_ = other.x6##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7)                                        \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    x4##_ = other.x4##_;                                                       \
                                                                               \
    x5##_ = other.x5##_;                                                       \
                                                                               \
    x6##_ = other.x6##_;                                                       \
                                                                               \
    x7##_ = other.x7##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_COPY_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7, T8, x8)                                \
                                                                               \
  inline name &operator=(name NO_SPEC() const &other) {                        \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    x4##_ = other.x4##_;                                                       \
                                                                               \
    x5##_ = other.x5##_;                                                       \
                                                                               \
    x6##_ = other.x6##_;                                                       \
                                                                               \
    x7##_ = other.x7##_;                                                       \
                                                                               \
    x8##_ = other.x8##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR() INST_ASSIGN_EXPR_EXP(NAME(), FIELDS())

#define INST_ASSIGN_EXPR_EXP(name, fields) INST_ASSIGN_EXPR_(name, fields)

#define INST_ASSIGN_EXPR_(...) VFUNC_ALT2(INST_ASSIGN_EXPR_, __VA_ARGS__)

#define INST_ASSIGN_EXPR_3(name, T1, x1)                                       \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_5(name, T1, x1, T2, x2)                               \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_7(name, T1, x1, T2, x2, T3, x3)                       \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_9(name, T1, x1, T2, x2, T3, x3, T4, x4)               \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    x4##_ = e.x4();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)      \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    x4##_ = e.x4();                                                            \
                                                                               \
    x5##_ = e.x5();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6)                                                \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    x4##_ = e.x4();                                                            \
                                                                               \
    x5##_ = e.x5();                                                            \
                                                                               \
    x6##_ = e.x6();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7)                                        \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    x4##_ = e.x4();                                                            \
                                                                               \
    x5##_ = e.x5();                                                            \
                                                                               \
    x6##_ = e.x6();                                                            \
                                                                               \
    x7##_ = e.x7();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_ASSIGN_EXPR_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7, T8, x8)                                \
                                                                               \
  template <typename E> inline name &operator=(name##_expr<E> const &e) {      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    x4##_ = e.x4();                                                            \
                                                                               \
    x5##_ = e.x5();                                                            \
                                                                               \
    x6##_ = e.x6();                                                            \
                                                                               \
    x7##_ = e.x7();                                                            \
                                                                               \
    x8##_ = e.x8();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define INST_LAZY_FIELDS() INST_LAZY_FIELDS_EXP(NAME(), FIELDS())

#define INST_LAZY_FIELDS_EXP(name, fields) INST_LAZY_FIELDS_(name, fields)

#define INST_LAZY_FIELDS_(...) VFUNC_ALT2(INST_LAZY_FIELDS_, __VA_ARGS__)

#define INST_LAZY_FIELDS_3(name, T1, x1)                                       \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }

#define INST_LAZY_FIELDS_5(name, T1, x1, T2, x2)                               \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }

#define INST_LAZY_FIELDS_7(name, T1, x1, T2, x2, T3, x3)                       \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }

#define INST_LAZY_FIELDS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)               \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }                                        \
                                                                               \
  inline const_ref<T4> x4() const { return x4##_; }                            \
  inline ref<T4> x4() { return x4##_; }

#define INST_LAZY_FIELDS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)      \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }                                        \
                                                                               \
  inline const_ref<T4> x4() const { return x4##_; }                            \
  inline ref<T4> x4() { return x4##_; }                                        \
                                                                               \
  inline const_ref<T5> x5() const { return x5##_; }                            \
  inline ref<T5> x5() { return x5##_; }

#define INST_LAZY_FIELDS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6)                                                \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }                                        \
                                                                               \
  inline const_ref<T4> x4() const { return x4##_; }                            \
  inline ref<T4> x4() { return x4##_; }                                        \
                                                                               \
  inline const_ref<T5> x5() const { return x5##_; }                            \
  inline ref<T5> x5() { return x5##_; }                                        \
                                                                               \
  inline const_ref<T6> x6() const { return x6##_; }                            \
  inline ref<T6> x6() { return x6##_; }

#define INST_LAZY_FIELDS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7)                                        \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }                                        \
                                                                               \
  inline const_ref<T4> x4() const { return x4##_; }                            \
  inline ref<T4> x4() { return x4##_; }                                        \
                                                                               \
  inline const_ref<T5> x5() const { return x5##_; }                            \
  inline ref<T5> x5() { return x5##_; }                                        \
                                                                               \
  inline const_ref<T6> x6() const { return x6##_; }                            \
  inline ref<T6> x6() { return x6##_; }                                        \
                                                                               \
  inline const_ref<T7> x7() const { return x7##_; }                            \
  inline ref<T7> x7() { return x7##_; }

#define INST_LAZY_FIELDS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,  \
                            x6, T7, x7, T8, x8)                                \
                                                                               \
  inline const_ref<T1> x1() const { return x1##_; }                            \
  inline ref<T1> x1() { return x1##_; }                                        \
                                                                               \
  inline const_ref<T2> x2() const { return x2##_; }                            \
  inline ref<T2> x2() { return x2##_; }                                        \
                                                                               \
  inline const_ref<T3> x3() const { return x3##_; }                            \
  inline ref<T3> x3() { return x3##_; }                                        \
                                                                               \
  inline const_ref<T4> x4() const { return x4##_; }                            \
  inline ref<T4> x4() { return x4##_; }                                        \
                                                                               \
  inline const_ref<T5> x5() const { return x5##_; }                            \
  inline ref<T5> x5() { return x5##_; }                                        \
                                                                               \
  inline const_ref<T6> x6() const { return x6##_; }                            \
  inline ref<T6> x6() { return x6##_; }                                        \
                                                                               \
  inline const_ref<T7> x7() const { return x7##_; }                            \
  inline ref<T7> x7() { return x7##_; }                                        \
                                                                               \
  inline const_ref<T8> x8() const { return x8##_; }                            \
  inline ref<T8> x8() { return x8##_; }

#define INST_FIELDS() INST_FIELDS_EXP(NAME(), FIELDS())

#define INST_FIELDS_EXP(name, fields) INST_FIELDS_(name, fields)

#define INST_FIELDS_(...) VFUNC_ALT2(INST_FIELDS_, __VA_ARGS__)

#define INST_FIELDS_3(name, T1, x1) T1 x1##_;

#define INST_FIELDS_5(name, T1, x1, T2, x2)                                    \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;

#define INST_FIELDS_7(name, T1, x1, T2, x2, T3, x3)                            \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;

#define INST_FIELDS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                    \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;                                                                    \
                                                                               \
  T4 x4##_;

#define INST_FIELDS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)           \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;                                                                    \
                                                                               \
  T4 x4##_;                                                                    \
                                                                               \
  T5 x5##_;

#define INST_FIELDS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)   \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;                                                                    \
                                                                               \
  T4 x4##_;                                                                    \
                                                                               \
  T5 x5##_;                                                                    \
                                                                               \
  T6 x6##_;

#define INST_FIELDS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,   \
                       T7, x7)                                                 \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;                                                                    \
                                                                               \
  T4 x4##_;                                                                    \
                                                                               \
  T5 x5##_;                                                                    \
                                                                               \
  T6 x6##_;                                                                    \
                                                                               \
  T7 x7##_;

#define INST_FIELDS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,   \
                       T7, x7, T8, x8)                                         \
                                                                               \
  T1 x1##_;                                                                    \
                                                                               \
  T2 x2##_;                                                                    \
                                                                               \
  T3 x3##_;                                                                    \
                                                                               \
  T4 x4##_;                                                                    \
                                                                               \
  T5 x5##_;                                                                    \
                                                                               \
  T6 x6##_;                                                                    \
                                                                               \
  T7 x7##_;                                                                    \
                                                                               \
  T8 x8##_;

#define INST_SERIALIZE() INST_SERIALIZE_EXP(NAME(), FIELDS())

#define INST_SERIALIZE_EXP(name, fields) INST_SERIALIZE_(name, fields)

#define INST_SERIALIZE_(...) VFUNC_ALT2(INST_SERIALIZE_, __VA_ARGS__)

#define INST_SERIALIZE_3(name, T1, x1)                                         \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
  }

#define INST_SERIALIZE_5(name, T1, x1, T2, x2)                                 \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
  }

#define INST_SERIALIZE_7(name, T1, x1, T2, x2, T3, x3)                         \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
  }

#define INST_SERIALIZE_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                 \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
                                                                               \
    ar &x4##_;                                                                 \
  }

#define INST_SERIALIZE_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)        \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
                                                                               \
    ar &x4##_;                                                                 \
                                                                               \
    ar &x5##_;                                                                 \
  }

#define INST_SERIALIZE_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,    \
                          x6)                                                  \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
                                                                               \
    ar &x4##_;                                                                 \
                                                                               \
    ar &x5##_;                                                                 \
                                                                               \
    ar &x6##_;                                                                 \
  }

#define INST_SERIALIZE_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,    \
                          x6, T7, x7)                                          \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
                                                                               \
    ar &x4##_;                                                                 \
                                                                               \
    ar &x5##_;                                                                 \
                                                                               \
    ar &x6##_;                                                                 \
                                                                               \
    ar &x7##_;                                                                 \
  }

#define INST_SERIALIZE_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,    \
                          x6, T7, x7, T8, x8)                                  \
                                                                               \
  friend class ::boost::serialization::access;                                 \
                                                                               \
  template <class Archive>                                                     \
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {   \
                                                                               \
    ar &x1##_;                                                                 \
                                                                               \
    ar &x2##_;                                                                 \
                                                                               \
    ar &x3##_;                                                                 \
                                                                               \
    ar &x4##_;                                                                 \
                                                                               \
    ar &x5##_;                                                                 \
                                                                               \
    ar &x6##_;                                                                 \
                                                                               \
    ar &x7##_;                                                                 \
                                                                               \
    ar &x8##_;                                                                 \
  }

#define INST_NAME() INST_NAME_EXP(NAME())

#define INST_NAME_EXP(name) INST_NAME_(name)

#define INST_NAME_(name)                                                       \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name : public name##_expr<name NO_SPEC()>

#define INST_BODY()                                                            \
                                                                               \
public:                                                                        \
  INST_CTORS()                                                                 \
  INST_ASSIGN_COPY()                                                           \
  INST_ASSIGN_EXPR()                                                           \
  INST_LAZY_FIELDS()                                                           \
                                                                               \
protected:                                                                     \
  INST_FIELDS()                                                                \
                                                                               \
private:                                                                       \
  INST_SERIALIZE()

#define ENTER_BOOST_SERIALIZATION_NS() namespace boost::serialization {
#define LEAVE_BOOST_SERIALIZATION_NS() }

#define INST_CONSTRUCT_DATA() INST_CONSTRUCT_DATA_EXP(NAME(), FIELDS())

#define INST_CONSTRUCT_DATA_EXP(name, fields) INST_CONSTRUCT_DATA_(name, fields)

#define INST_CONSTRUCT_DATA_(...) VFUNC_ALT2(INST_CONSTRUCT_DATA_, __VA_ARGS__)

#define INST_CONSTRUCT_DATA_3(name, T1, x1)                                    \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_5(name, T1, x1, T2, x2)                            \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_7(name, T1, x1, T2, x2, T3, x3)                    \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_9(name, T1, x1, T2, x2, T3, x3, T4, x4)            \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
                                                                               \
    T4 x4##_inst = inst->x4();                                                 \
    save_construct_data(ar, &x4##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T4)) unsigned char x4##_buf[sizeof(T4)];                   \
    T4 &x4##_ref = *reinterpret_cast<T4 *>(&x4##_buf[0]);                      \
    ar >> x4##_ref;                                                            \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref                                                               \
                                                                               \
        ,                                                                      \
        x4##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)   \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
                                                                               \
    T4 x4##_inst = inst->x4();                                                 \
    save_construct_data(ar, &x4##_inst, version);                              \
                                                                               \
    T5 x5##_inst = inst->x5();                                                 \
    save_construct_data(ar, &x5##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T5)) unsigned char x5##_buf[sizeof(T5)];                   \
    T5 &x5##_ref = *reinterpret_cast<T5 *>(&x5##_buf[0]);                      \
    ar >> x5##_ref;                                                            \
                                                                               \
    alignas(alignof(T4)) unsigned char x4##_buf[sizeof(T4)];                   \
    T4 &x4##_ref = *reinterpret_cast<T4 *>(&x4##_buf[0]);                      \
    ar >> x4##_ref;                                                            \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref                                                               \
                                                                               \
        ,                                                                      \
        x4##_ref                                                               \
                                                                               \
        ,                                                                      \
        x5##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5,   \
                               T6, x6)                                         \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
                                                                               \
    T4 x4##_inst = inst->x4();                                                 \
    save_construct_data(ar, &x4##_inst, version);                              \
                                                                               \
    T5 x5##_inst = inst->x5();                                                 \
    save_construct_data(ar, &x5##_inst, version);                              \
                                                                               \
    T6 x6##_inst = inst->x6();                                                 \
    save_construct_data(ar, &x6##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T6)) unsigned char x6##_buf[sizeof(T6)];                   \
    T6 &x6##_ref = *reinterpret_cast<T6 *>(&x6##_buf[0]);                      \
    ar >> x6##_ref;                                                            \
                                                                               \
    alignas(alignof(T5)) unsigned char x5##_buf[sizeof(T5)];                   \
    T5 &x5##_ref = *reinterpret_cast<T5 *>(&x5##_buf[0]);                      \
    ar >> x5##_ref;                                                            \
                                                                               \
    alignas(alignof(T4)) unsigned char x4##_buf[sizeof(T4)];                   \
    T4 &x4##_ref = *reinterpret_cast<T4 *>(&x4##_buf[0]);                      \
    ar >> x4##_ref;                                                            \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref                                                               \
                                                                               \
        ,                                                                      \
        x4##_ref                                                               \
                                                                               \
        ,                                                                      \
        x5##_ref                                                               \
                                                                               \
        ,                                                                      \
        x6##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5,   \
                               T6, x6, T7, x7)                                 \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
                                                                               \
    T4 x4##_inst = inst->x4();                                                 \
    save_construct_data(ar, &x4##_inst, version);                              \
                                                                               \
    T5 x5##_inst = inst->x5();                                                 \
    save_construct_data(ar, &x5##_inst, version);                              \
                                                                               \
    T6 x6##_inst = inst->x6();                                                 \
    save_construct_data(ar, &x6##_inst, version);                              \
                                                                               \
    T7 x7##_inst = inst->x7();                                                 \
    save_construct_data(ar, &x7##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T7)) unsigned char x7##_buf[sizeof(T7)];                   \
    T7 &x7##_ref = *reinterpret_cast<T7 *>(&x7##_buf[0]);                      \
    ar >> x7##_ref;                                                            \
                                                                               \
    alignas(alignof(T6)) unsigned char x6##_buf[sizeof(T6)];                   \
    T6 &x6##_ref = *reinterpret_cast<T6 *>(&x6##_buf[0]);                      \
    ar >> x6##_ref;                                                            \
                                                                               \
    alignas(alignof(T5)) unsigned char x5##_buf[sizeof(T5)];                   \
    T5 &x5##_ref = *reinterpret_cast<T5 *>(&x5##_buf[0]);                      \
    ar >> x5##_ref;                                                            \
                                                                               \
    alignas(alignof(T4)) unsigned char x4##_buf[sizeof(T4)];                   \
    T4 &x4##_ref = *reinterpret_cast<T4 *>(&x4##_buf[0]);                      \
    ar >> x4##_ref;                                                            \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref                                                               \
                                                                               \
        ,                                                                      \
        x4##_ref                                                               \
                                                                               \
        ,                                                                      \
        x5##_ref                                                               \
                                                                               \
        ,                                                                      \
        x6##_ref                                                               \
                                                                               \
        ,                                                                      \
        x7##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define INST_CONSTRUCT_DATA_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5,   \
                               T6, x6, T7, x7, T8, x8)                         \
                                                                               \
  ENTER_BOOST_SERIALIZATION_NS()                                               \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void save_construct_data(Archive &ar,                                        \
                           NS_PREFIX() name NO_SPEC() const *inst,             \
                           const unsigned int version) {                       \
                                                                               \
    T1 x1##_inst = inst->x1();                                                 \
    save_construct_data(ar, &x1##_inst, version);                              \
                                                                               \
    T2 x2##_inst = inst->x2();                                                 \
    save_construct_data(ar, &x2##_inst, version);                              \
                                                                               \
    T3 x3##_inst = inst->x3();                                                 \
    save_construct_data(ar, &x3##_inst, version);                              \
                                                                               \
    T4 x4##_inst = inst->x4();                                                 \
    save_construct_data(ar, &x4##_inst, version);                              \
                                                                               \
    T5 x5##_inst = inst->x5();                                                 \
    save_construct_data(ar, &x5##_inst, version);                              \
                                                                               \
    T6 x6##_inst = inst->x6();                                                 \
    save_construct_data(ar, &x6##_inst, version);                              \
                                                                               \
    T7 x7##_inst = inst->x7();                                                 \
    save_construct_data(ar, &x7##_inst, version);                              \
                                                                               \
    T8 x8##_inst = inst->x8();                                                 \
    save_construct_data(ar, &x8##_inst, version);                              \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, Archive)                                                  \
  void load_construct_data(Archive &ar, NS_PREFIX() name NO_SPEC() * inst,     \
                           [[maybe_unused]] const unsigned int version) {      \
                                                                               \
    alignas(alignof(T8)) unsigned char x8##_buf[sizeof(T8)];                   \
    T8 &x8##_ref = *reinterpret_cast<T8 *>(&x8##_buf[0]);                      \
    ar >> x8##_ref;                                                            \
                                                                               \
    alignas(alignof(T7)) unsigned char x7##_buf[sizeof(T7)];                   \
    T7 &x7##_ref = *reinterpret_cast<T7 *>(&x7##_buf[0]);                      \
    ar >> x7##_ref;                                                            \
                                                                               \
    alignas(alignof(T6)) unsigned char x6##_buf[sizeof(T6)];                   \
    T6 &x6##_ref = *reinterpret_cast<T6 *>(&x6##_buf[0]);                      \
    ar >> x6##_ref;                                                            \
                                                                               \
    alignas(alignof(T5)) unsigned char x5##_buf[sizeof(T5)];                   \
    T5 &x5##_ref = *reinterpret_cast<T5 *>(&x5##_buf[0]);                      \
    ar >> x5##_ref;                                                            \
                                                                               \
    alignas(alignof(T4)) unsigned char x4##_buf[sizeof(T4)];                   \
    T4 &x4##_ref = *reinterpret_cast<T4 *>(&x4##_buf[0]);                      \
    ar >> x4##_ref;                                                            \
                                                                               \
    alignas(alignof(T3)) unsigned char x3##_buf[sizeof(T3)];                   \
    T3 &x3##_ref = *reinterpret_cast<T3 *>(&x3##_buf[0]);                      \
    ar >> x3##_ref;                                                            \
                                                                               \
    alignas(alignof(T2)) unsigned char x2##_buf[sizeof(T2)];                   \
    T2 &x2##_ref = *reinterpret_cast<T2 *>(&x2##_buf[0]);                      \
    ar >> x2##_ref;                                                            \
                                                                               \
    alignas(alignof(T1)) unsigned char x1##_buf[sizeof(T1)];                   \
    T1 &x1##_ref = *reinterpret_cast<T1 *>(&x1##_buf[0]);                      \
    ar >> x1##_ref;                                                            \
                                                                               \
    ::new (static_cast<void *>(inst)) NS_PREFIX() name NO_SPEC()(              \
                                                                               \
        x1##_ref                                                               \
                                                                               \
        ,                                                                      \
        x2##_ref                                                               \
                                                                               \
        ,                                                                      \
        x3##_ref                                                               \
                                                                               \
        ,                                                                      \
        x4##_ref                                                               \
                                                                               \
        ,                                                                      \
        x5##_ref                                                               \
                                                                               \
        ,                                                                      \
        x6##_ref                                                               \
                                                                               \
        ,                                                                      \
        x7##_ref                                                               \
                                                                               \
        ,                                                                      \
        x8##_ref);                                                             \
  }                                                                            \
                                                                               \
  LEAVE_BOOST_SERIALIZATION_NS()

#define GEN_INST()                                                             \
                                                                               \
  ENTER_NS()                                                                   \
  INST_NAME(){INST_BODY()};                                                    \
  LEAVE_NS()                                                                   \
  INST_CONSTRUCT_DATA()
