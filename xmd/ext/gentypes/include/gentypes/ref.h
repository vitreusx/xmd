

#pragma once
#include "meta.h"

template <typename T> struct ref_impl { using type = T &; };

template <typename T> using ref = typename ref_impl<T>::type;

#define REF_CTORS() REF_CTORS_EXP(NAME(), FIELDS())

#define REF_CTORS_EXP(name, fields) REF_CTORS_(name, fields)

#define REF_CTORS_(...) VFUNC_ALT2(REF_CTORS_, __VA_ARGS__)

#define REF_CTORS_3(name, T1, x1)                                              \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        {};

#define REF_CTORS_5(name, T1, x1, T2, x2)                                      \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
      :                                                                        \
                                                                               \
        x1##_{other.x1##_}                                                     \
                                                                               \
        ,                                                                      \
        x2##_{other.x2##_}                                                     \
                                                                               \
        {};

#define REF_CTORS_7(name, T1, x1, T2, x2, T3, x3)                              \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_CTORS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                      \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      ,                                                                        \
      ref<T4> x4                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        ,                                                                      \
        x4##_{x4}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        ,                                                                      \
        x4##_{inst_.x4()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_CTORS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)             \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      ,                                                                        \
      ref<T4> x4                                                               \
                                                                               \
      ,                                                                        \
      ref<T5> x5                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        ,                                                                      \
        x4##_{x4}                                                              \
                                                                               \
        ,                                                                      \
        x5##_{x5}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        ,                                                                      \
        x4##_{inst_.x4()}                                                      \
                                                                               \
        ,                                                                      \
        x5##_{inst_.x5()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_CTORS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)     \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      ,                                                                        \
      ref<T4> x4                                                               \
                                                                               \
      ,                                                                        \
      ref<T5> x5                                                               \
                                                                               \
      ,                                                                        \
      ref<T6> x6                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        ,                                                                      \
        x4##_{x4}                                                              \
                                                                               \
        ,                                                                      \
        x5##_{x5}                                                              \
                                                                               \
        ,                                                                      \
        x6##_{x6}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        ,                                                                      \
        x4##_{inst_.x4()}                                                      \
                                                                               \
        ,                                                                      \
        x5##_{inst_.x5()}                                                      \
                                                                               \
        ,                                                                      \
        x6##_{inst_.x6()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_CTORS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7, \
                     x7)                                                       \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      ,                                                                        \
      ref<T4> x4                                                               \
                                                                               \
      ,                                                                        \
      ref<T5> x5                                                               \
                                                                               \
      ,                                                                        \
      ref<T6> x6                                                               \
                                                                               \
      ,                                                                        \
      ref<T7> x7                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        ,                                                                      \
        x4##_{x4}                                                              \
                                                                               \
        ,                                                                      \
        x5##_{x5}                                                              \
                                                                               \
        ,                                                                      \
        x6##_{x6}                                                              \
                                                                               \
        ,                                                                      \
        x7##_{x7}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        ,                                                                      \
        x4##_{inst_.x4()}                                                      \
                                                                               \
        ,                                                                      \
        x5##_{inst_.x5()}                                                      \
                                                                               \
        ,                                                                      \
        x6##_{inst_.x6()}                                                      \
                                                                               \
        ,                                                                      \
        x7##_{inst_.x7()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_CTORS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7, \
                     x7, T8, x8)                                               \
                                                                               \
  inline name##_ref(                                                           \
                                                                               \
      ref<T1> x1                                                               \
                                                                               \
      ,                                                                        \
      ref<T2> x2                                                               \
                                                                               \
      ,                                                                        \
      ref<T3> x3                                                               \
                                                                               \
      ,                                                                        \
      ref<T4> x4                                                               \
                                                                               \
      ,                                                                        \
      ref<T5> x5                                                               \
                                                                               \
      ,                                                                        \
      ref<T6> x6                                                               \
                                                                               \
      ,                                                                        \
      ref<T7> x7                                                               \
                                                                               \
      ,                                                                        \
      ref<T8> x8                                                               \
                                                                               \
      )                                                                        \
      :                                                                        \
                                                                               \
        x1##_{x1}                                                              \
                                                                               \
        ,                                                                      \
        x2##_{x2}                                                              \
                                                                               \
        ,                                                                      \
        x3##_{x3}                                                              \
                                                                               \
        ,                                                                      \
        x4##_{x4}                                                              \
                                                                               \
        ,                                                                      \
        x5##_{x5}                                                              \
                                                                               \
        ,                                                                      \
        x6##_{x6}                                                              \
                                                                               \
        ,                                                                      \
        x7##_{x7}                                                              \
                                                                               \
        ,                                                                      \
        x8##_{x8}                                                              \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name NO_SPEC() & inst_)                                    \
      :                                                                        \
                                                                               \
        x1##_{inst_.x1()}                                                      \
                                                                               \
        ,                                                                      \
        x2##_{inst_.x2()}                                                      \
                                                                               \
        ,                                                                      \
        x3##_{inst_.x3()}                                                      \
                                                                               \
        ,                                                                      \
        x4##_{inst_.x4()}                                                      \
                                                                               \
        ,                                                                      \
        x5##_{inst_.x5()}                                                      \
                                                                               \
        ,                                                                      \
        x6##_{inst_.x6()}                                                      \
                                                                               \
        ,                                                                      \
        x7##_{inst_.x7()}                                                      \
                                                                               \
        ,                                                                      \
        x8##_{inst_.x8()}                                                      \
                                                                               \
        {};                                                                    \
                                                                               \
  inline name##_ref(name##_ref NO_SPEC() const &other)                         \
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
        {};

#define REF_ASSIGN_COPY() REF_ASSIGN_COPY_EXP(NAME(), FIELDS())

#define REF_ASSIGN_COPY_EXP(name, fields) REF_ASSIGN_COPY_(name, fields)

#define REF_ASSIGN_COPY_(...) VFUNC_ALT2(REF_ASSIGN_COPY_, __VA_ARGS__)

#define REF_ASSIGN_COPY_3(name, T1, x1)                                        \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_COPY_5(name, T1, x1, T2, x2)                                \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_COPY_7(name, T1, x1, T2, x2, T3, x3)                        \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
                                                                               \
    x1##_ = other.x1##_;                                                       \
                                                                               \
    x2##_ = other.x2##_;                                                       \
                                                                               \
    x3##_ = other.x3##_;                                                       \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_COPY_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
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

#define REF_ASSIGN_COPY_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)       \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
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

#define REF_ASSIGN_COPY_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6)                                                 \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
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

#define REF_ASSIGN_COPY_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7)                                         \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
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

#define REF_ASSIGN_COPY_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7, T8, x8)                                 \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() const &other) {            \
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

#define REF_ASSIGN_MOVE() REF_ASSIGN_MOVE_EXP(NAME(), FIELDS())

#define REF_ASSIGN_MOVE_EXP(name, fields) REF_ASSIGN_MOVE_(name, fields)

#define REF_ASSIGN_MOVE_(...) VFUNC_ALT2(REF_ASSIGN_MOVE_, __VA_ARGS__)

#define REF_ASSIGN_MOVE_3(name, T1, x1)                                        \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_5(name, T1, x1, T2, x2)                                \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_7(name, T1, x1, T2, x2, T3, x3)                        \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    x4##_ = std::move(other.x4##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)       \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    x4##_ = std::move(other.x4##_);                                            \
                                                                               \
    x5##_ = std::move(other.x5##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6)                                                 \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    x4##_ = std::move(other.x4##_);                                            \
                                                                               \
    x5##_ = std::move(other.x5##_);                                            \
                                                                               \
    x6##_ = std::move(other.x6##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7)                                         \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    x4##_ = std::move(other.x4##_);                                            \
                                                                               \
    x5##_ = std::move(other.x5##_);                                            \
                                                                               \
    x6##_ = std::move(other.x6##_);                                            \
                                                                               \
    x7##_ = std::move(other.x7##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_MOVE_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7, T8, x8)                                 \
                                                                               \
  inline name##_ref &operator=(name##_ref NO_SPEC() && other) {                \
                                                                               \
    x1##_ = std::move(other.x1##_);                                            \
                                                                               \
    x2##_ = std::move(other.x2##_);                                            \
                                                                               \
    x3##_ = std::move(other.x3##_);                                            \
                                                                               \
    x4##_ = std::move(other.x4##_);                                            \
                                                                               \
    x5##_ = std::move(other.x5##_);                                            \
                                                                               \
    x6##_ = std::move(other.x6##_);                                            \
                                                                               \
    x7##_ = std::move(other.x7##_);                                            \
                                                                               \
    x8##_ = std::move(other.x8##_);                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_EXPR() REF_ASSIGN_EXPR_EXP(NAME(), FIELDS())

#define REF_ASSIGN_EXPR_EXP(name, fields) REF_ASSIGN_EXPR_(name, fields)

#define REF_ASSIGN_EXPR_(...) VFUNC_ALT2(REF_ASSIGN_EXPR_, __VA_ARGS__)

#define REF_ASSIGN_EXPR_3(name, T1, x1)                                        \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_EXPR_5(name, T1, x1, T2, x2)                                \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_EXPR_7(name, T1, x1, T2, x2, T3, x3)                        \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
                                                                               \
    x1##_ = e.x1();                                                            \
                                                                               \
    x2##_ = e.x2();                                                            \
                                                                               \
    x3##_ = e.x3();                                                            \
                                                                               \
    return *this;                                                              \
  }

#define REF_ASSIGN_EXPR_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
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

#define REF_ASSIGN_EXPR_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)       \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
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

#define REF_ASSIGN_EXPR_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6)                                                 \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
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

#define REF_ASSIGN_EXPR_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7)                                         \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
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

#define REF_ASSIGN_EXPR_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7, T8, x8)                                 \
                                                                               \
  template <typename E>                                                        \
  inline name##_ref &operator=(name##_expr<E> const &e) {                      \
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

#define REF_SWAP() REF_SWAP_EXP(NAME(), FIELDS())

#define REF_SWAP_EXP(name, fields) REF_SWAP_(name, fields)

#define REF_SWAP_(...) VFUNC_ALT2(REF_SWAP_, __VA_ARGS__)

#define REF_SWAP_3(name, T1, x1)                                               \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
  }

#define REF_SWAP_5(name, T1, x1, T2, x2)                                       \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
  }

#define REF_SWAP_7(name, T1, x1, T2, x2, T3, x3)                               \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
  }

#define REF_SWAP_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                       \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
                                                                               \
    swap(a.x4##_, b.x4##_);                                                    \
  }

#define REF_SWAP_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)              \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
                                                                               \
    swap(a.x4##_, b.x4##_);                                                    \
                                                                               \
    swap(a.x5##_, b.x5##_);                                                    \
  }

#define REF_SWAP_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)      \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
                                                                               \
    swap(a.x4##_, b.x4##_);                                                    \
                                                                               \
    swap(a.x5##_, b.x5##_);                                                    \
                                                                               \
    swap(a.x6##_, b.x6##_);                                                    \
  }

#define REF_SWAP_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7)                                                        \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
                                                                               \
    swap(a.x4##_, b.x4##_);                                                    \
                                                                               \
    swap(a.x5##_, b.x5##_);                                                    \
                                                                               \
    swap(a.x6##_, b.x6##_);                                                    \
                                                                               \
    swap(a.x7##_, b.x7##_);                                                    \
  }

#define REF_SWAP_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,  \
                    x7, T8, x8)                                                \
                                                                               \
  friend void swap(name##_ref const &a, name##_ref const &b) {                 \
    using std::swap;                                                           \
                                                                               \
    swap(a.x1##_, b.x1##_);                                                    \
                                                                               \
    swap(a.x2##_, b.x2##_);                                                    \
                                                                               \
    swap(a.x3##_, b.x3##_);                                                    \
                                                                               \
    swap(a.x4##_, b.x4##_);                                                    \
                                                                               \
    swap(a.x5##_, b.x5##_);                                                    \
                                                                               \
    swap(a.x6##_, b.x6##_);                                                    \
                                                                               \
    swap(a.x7##_, b.x7##_);                                                    \
                                                                               \
    swap(a.x8##_, b.x8##_);                                                    \
  }

#define REF_LAZY_FIELDS() REF_LAZY_FIELDS_EXP(NAME(), FIELDS())

#define REF_LAZY_FIELDS_EXP(name, fields) REF_LAZY_FIELDS_(name, fields)

#define REF_LAZY_FIELDS_(...) VFUNC_ALT2(REF_LAZY_FIELDS_, __VA_ARGS__)

#define REF_LAZY_FIELDS_3(name, T1, x1)                                        \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }

#define REF_LAZY_FIELDS_5(name, T1, x1, T2, x2)                                \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }

#define REF_LAZY_FIELDS_7(name, T1, x1, T2, x2, T3, x3)                        \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }

#define REF_LAZY_FIELDS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }                                  \
                                                                               \
  inline ref<T4> x4() const { return x4##_; }

#define REF_LAZY_FIELDS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)       \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }                                  \
                                                                               \
  inline ref<T4> x4() const { return x4##_; }                                  \
                                                                               \
  inline ref<T5> x5() const { return x5##_; }

#define REF_LAZY_FIELDS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6)                                                 \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }                                  \
                                                                               \
  inline ref<T4> x4() const { return x4##_; }                                  \
                                                                               \
  inline ref<T5> x5() const { return x5##_; }                                  \
                                                                               \
  inline ref<T6> x6() const { return x6##_; }

#define REF_LAZY_FIELDS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7)                                         \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }                                  \
                                                                               \
  inline ref<T4> x4() const { return x4##_; }                                  \
                                                                               \
  inline ref<T5> x5() const { return x5##_; }                                  \
                                                                               \
  inline ref<T6> x6() const { return x6##_; }                                  \
                                                                               \
  inline ref<T7> x7() const { return x7##_; }

#define REF_LAZY_FIELDS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6,   \
                           x6, T7, x7, T8, x8)                                 \
                                                                               \
  inline ref<T1> x1() const { return x1##_; }                                  \
                                                                               \
  inline ref<T2> x2() const { return x2##_; }                                  \
                                                                               \
  inline ref<T3> x3() const { return x3##_; }                                  \
                                                                               \
  inline ref<T4> x4() const { return x4##_; }                                  \
                                                                               \
  inline ref<T5> x5() const { return x5##_; }                                  \
                                                                               \
  inline ref<T6> x6() const { return x6##_; }                                  \
                                                                               \
  inline ref<T7> x7() const { return x7##_; }                                  \
                                                                               \
  inline ref<T8> x8() const { return x8##_; }

#define REF_FIELDS() REF_FIELDS_EXP(NAME(), FIELDS())

#define REF_FIELDS_EXP(name, fields) REF_FIELDS_(name, fields)

#define REF_FIELDS_(...) VFUNC_ALT2(REF_FIELDS_, __VA_ARGS__)

#define REF_FIELDS_3(name, T1, x1) ref<T1> x1##_;

#define REF_FIELDS_5(name, T1, x1, T2, x2)                                     \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;

#define REF_FIELDS_7(name, T1, x1, T2, x2, T3, x3)                             \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;

#define REF_FIELDS_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                     \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;                                                               \
                                                                               \
  ref<T4> x4##_;

#define REF_FIELDS_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)            \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;                                                               \
                                                                               \
  ref<T4> x4##_;                                                               \
                                                                               \
  ref<T5> x5##_;

#define REF_FIELDS_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)    \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;                                                               \
                                                                               \
  ref<T4> x4##_;                                                               \
                                                                               \
  ref<T5> x5##_;                                                               \
                                                                               \
  ref<T6> x6##_;

#define REF_FIELDS_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7)                                                  \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;                                                               \
                                                                               \
  ref<T4> x4##_;                                                               \
                                                                               \
  ref<T5> x5##_;                                                               \
                                                                               \
  ref<T6> x6##_;                                                               \
                                                                               \
  ref<T7> x7##_;

#define REF_FIELDS_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7, T8, x8)                                          \
                                                                               \
  ref<T1> x1##_;                                                               \
                                                                               \
  ref<T2> x2##_;                                                               \
                                                                               \
  ref<T3> x3##_;                                                               \
                                                                               \
  ref<T4> x4##_;                                                               \
                                                                               \
  ref<T5> x5##_;                                                               \
                                                                               \
  ref<T6> x6##_;                                                               \
                                                                               \
  ref<T7> x7##_;                                                               \
                                                                               \
  ref<T8> x8##_;

#define REF_IMPL_SPEC() REF_IMPL_SPEC_EXP(NAME())

#define REF_IMPL_SPEC_EXP(name) REF_IMPL_SPEC_(name)

#define REF_IMPL_SPEC_(name)                                                   \
                                                                               \
  NULL_SPEC()                                                                  \
  struct ref_impl<NS_PREFIX() name NO_SPEC()> {                                \
    using type = NS_PREFIX() name##_ref NO_SPEC();                             \
  };

#define REF_NAME() REF_NAME_EXP(NAME())

#define REF_NAME_EXP(name) REF_NAME_(name)

#define REF_NAME_(name)                                                        \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_ref : public name##_expr<name##_ref NO_SPEC()>

#define REF_BODY()                                                             \
                                                                               \
public:                                                                        \
  REF_CTORS()                                                                  \
  REF_ASSIGN_COPY()                                                            \
  REF_ASSIGN_MOVE()                                                            \
  REF_ASSIGN_EXPR()                                                            \
  REF_SWAP()                                                                   \
  REF_LAZY_FIELDS()                                                            \
                                                                               \
protected:                                                                     \
  REF_FIELDS()

#define GEN_REF() GEN_REF_EXP(NAME(), FIELDS())

#define GEN_REF_EXP(name, fields) GEN_REF_(name, fields)

#define GEN_REF_(...) VFUNC(GEN_REF_, __VA_ARGS__)

#define GEN_REF_3(name, T1, x1)                                                \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_5(name, T1, x1, T2, x2)                                        \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_7(name, T1, x1, T2, x2, T3, x3)                                \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                        \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)               \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)       \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7)                                                         \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()

#define GEN_REF_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, T7,   \
                   x7, T8, x8)                                                 \
                                                                               \
  ENTER_NS()                                                                   \
  REF_NAME(){REF_BODY()};                                                      \
  LEAVE_NS()                                                                   \
                                                                               \
  REF_IMPL_SPEC()
