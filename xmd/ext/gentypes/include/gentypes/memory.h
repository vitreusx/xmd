

#pragma once
#include "def_memory.h"
#include "meta.h"

template <typename T> struct destroy_at {
  static inline void impl(T *p) { std::destroy_at(p); }
};

#define GEN_MEMORY() GEN_MEMORY_EXP(NAME(), FIELDS())

#define GEN_MEMORY_EXP(name, fields) GEN_MEMORY_(name, fields)

#define GEN_MEMORY_(...) VFUNC(GEN_MEMORY_, __VA_ARGS__)

#define GEN_MEMORY_3(name, T1, x1)                                             \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_5(name, T1, x1, T2, x2)                                     \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_7(name, T1, x1, T2, x2, T3, x3)                             \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                     \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
                                                                               \
      uninitialized_fill_n<T4>::impl(p.x4##_ptr, n, e.x4());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_copy_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_move_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
                                                                               \
      destroy_n<T4>::impl(p.x4##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
                                                                               \
      construct_at<T4>::impl(p.x4##_ptr, e.x4());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
                                                                               \
      destroy_at<T4>::impl(p.x4##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)            \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
                                                                               \
      uninitialized_fill_n<T4>::impl(p.x4##_ptr, n, e.x4());                   \
                                                                               \
      uninitialized_fill_n<T5>::impl(p.x5##_ptr, n, e.x5());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_copy_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_copy_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_move_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_move_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
                                                                               \
      destroy_n<T4>::impl(p.x4##_ptr, n);                                      \
                                                                               \
      destroy_n<T5>::impl(p.x5##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
                                                                               \
      construct_at<T4>::impl(p.x4##_ptr, e.x4());                              \
                                                                               \
      construct_at<T5>::impl(p.x5##_ptr, e.x5());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
                                                                               \
      destroy_at<T4>::impl(p.x4##_ptr);                                        \
                                                                               \
      destroy_at<T5>::impl(p.x5##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)    \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
                                                                               \
      uninitialized_fill_n<T4>::impl(p.x4##_ptr, n, e.x4());                   \
                                                                               \
      uninitialized_fill_n<T5>::impl(p.x5##_ptr, n, e.x5());                   \
                                                                               \
      uninitialized_fill_n<T6>::impl(p.x6##_ptr, n, e.x6());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_copy_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_copy_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_copy_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_move_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_move_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_move_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
                                                                               \
      destroy_n<T4>::impl(p.x4##_ptr, n);                                      \
                                                                               \
      destroy_n<T5>::impl(p.x5##_ptr, n);                                      \
                                                                               \
      destroy_n<T6>::impl(p.x6##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
                                                                               \
      construct_at<T4>::impl(p.x4##_ptr, e.x4());                              \
                                                                               \
      construct_at<T5>::impl(p.x5##_ptr, e.x5());                              \
                                                                               \
      construct_at<T6>::impl(p.x6##_ptr, e.x6());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
                                                                               \
      destroy_at<T4>::impl(p.x4##_ptr);                                        \
                                                                               \
      destroy_at<T5>::impl(p.x5##_ptr);                                        \
                                                                               \
      destroy_at<T6>::impl(p.x6##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7)                                                  \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
                                                                               \
      uninitialized_fill_n<T4>::impl(p.x4##_ptr, n, e.x4());                   \
                                                                               \
      uninitialized_fill_n<T5>::impl(p.x5##_ptr, n, e.x5());                   \
                                                                               \
      uninitialized_fill_n<T6>::impl(p.x6##_ptr, n, e.x6());                   \
                                                                               \
      uninitialized_fill_n<T7>::impl(p.x7##_ptr, n, e.x7());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_copy_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_copy_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_copy_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
                                                                               \
      uninitialized_copy_n<T7>::impl(from.x7##_ptr, n, to.x7##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_move_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_move_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_move_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
                                                                               \
      uninitialized_move_n<T7>::impl(from.x7##_ptr, n, to.x7##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
                                                                               \
      destroy_n<T4>::impl(p.x4##_ptr, n);                                      \
                                                                               \
      destroy_n<T5>::impl(p.x5##_ptr, n);                                      \
                                                                               \
      destroy_n<T6>::impl(p.x6##_ptr, n);                                      \
                                                                               \
      destroy_n<T7>::impl(p.x7##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
                                                                               \
      construct_at<T4>::impl(p.x4##_ptr, e.x4());                              \
                                                                               \
      construct_at<T5>::impl(p.x5##_ptr, e.x5());                              \
                                                                               \
      construct_at<T6>::impl(p.x6##_ptr, e.x6());                              \
                                                                               \
      construct_at<T7>::impl(p.x7##_ptr, e.x7());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
                                                                               \
      destroy_at<T4>::impl(p.x4##_ptr);                                        \
                                                                               \
      destroy_at<T5>::impl(p.x5##_ptr);                                        \
                                                                               \
      destroy_at<T6>::impl(p.x6##_ptr);                                        \
                                                                               \
      destroy_at<T7>::impl(p.x7##_ptr);                                        \
    }                                                                          \
  };

#define GEN_MEMORY_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7, T8, x8)                                          \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_fill_n<NS_PREFIX() name NO_SPEC()> {                    \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n, NS_PREFIX() name##_expr<E> const &e) {   \
                                                                               \
      uninitialized_fill_n<T1>::impl(p.x1##_ptr, n, e.x1());                   \
                                                                               \
      uninitialized_fill_n<T2>::impl(p.x2##_ptr, n, e.x2());                   \
                                                                               \
      uninitialized_fill_n<T3>::impl(p.x3##_ptr, n, e.x3());                   \
                                                                               \
      uninitialized_fill_n<T4>::impl(p.x4##_ptr, n, e.x4());                   \
                                                                               \
      uninitialized_fill_n<T5>::impl(p.x5##_ptr, n, e.x5());                   \
                                                                               \
      uninitialized_fill_n<T6>::impl(p.x6##_ptr, n, e.x6());                   \
                                                                               \
      uninitialized_fill_n<T7>::impl(p.x7##_ptr, n, e.x7());                   \
                                                                               \
      uninitialized_fill_n<T8>::impl(p.x8##_ptr, n, e.x8());                   \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_copy_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_const_ptr NO_SPEC()             \
                                const &from,                                   \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_copy_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_copy_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_copy_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_copy_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_copy_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_copy_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
                                                                               \
      uninitialized_copy_n<T7>::impl(from.x7##_ptr, n, to.x7##_ptr);           \
                                                                               \
      uninitialized_copy_n<T8>::impl(from.x8##_ptr, n, to.x8##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct uninitialized_move_n<NS_PREFIX() name NO_SPEC()> {                    \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &from,      \
                            size_t n,                                          \
                            NS_PREFIX() name##_ptr NO_SPEC() const &to) {      \
                                                                               \
      uninitialized_move_n<T1>::impl(from.x1##_ptr, n, to.x1##_ptr);           \
                                                                               \
      uninitialized_move_n<T2>::impl(from.x2##_ptr, n, to.x2##_ptr);           \
                                                                               \
      uninitialized_move_n<T3>::impl(from.x3##_ptr, n, to.x3##_ptr);           \
                                                                               \
      uninitialized_move_n<T4>::impl(from.x4##_ptr, n, to.x4##_ptr);           \
                                                                               \
      uninitialized_move_n<T5>::impl(from.x5##_ptr, n, to.x5##_ptr);           \
                                                                               \
      uninitialized_move_n<T6>::impl(from.x6##_ptr, n, to.x6##_ptr);           \
                                                                               \
      uninitialized_move_n<T7>::impl(from.x7##_ptr, n, to.x7##_ptr);           \
                                                                               \
      uninitialized_move_n<T8>::impl(from.x8##_ptr, n, to.x8##_ptr);           \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_n<NS_PREFIX() name NO_SPEC()> {                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            size_t n) {                                        \
                                                                               \
      destroy_n<T1>::impl(p.x1##_ptr, n);                                      \
                                                                               \
      destroy_n<T2>::impl(p.x2##_ptr, n);                                      \
                                                                               \
      destroy_n<T3>::impl(p.x3##_ptr, n);                                      \
                                                                               \
      destroy_n<T4>::impl(p.x4##_ptr, n);                                      \
                                                                               \
      destroy_n<T5>::impl(p.x5##_ptr, n);                                      \
                                                                               \
      destroy_n<T6>::impl(p.x6##_ptr, n);                                      \
                                                                               \
      destroy_n<T7>::impl(p.x7##_ptr, n);                                      \
                                                                               \
      destroy_n<T8>::impl(p.x8##_ptr, n);                                      \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct construct_at<NS_PREFIX() name NO_SPEC()> {                            \
    template <typename E>                                                      \
    static inline void _impl_expr(NS_PREFIX() name##_ptr NO_SPEC() const &p,   \
                                  NS_PREFIX() name##_expr<E> const &e) {       \
                                                                               \
      construct_at<T1>::impl(p.x1##_ptr, e.x1());                              \
                                                                               \
      construct_at<T2>::impl(p.x2##_ptr, e.x2());                              \
                                                                               \
      construct_at<T3>::impl(p.x3##_ptr, e.x3());                              \
                                                                               \
      construct_at<T4>::impl(p.x4##_ptr, e.x4());                              \
                                                                               \
      construct_at<T5>::impl(p.x5##_ptr, e.x5());                              \
                                                                               \
      construct_at<T6>::impl(p.x6##_ptr, e.x6());                              \
                                                                               \
      construct_at<T7>::impl(p.x7##_ptr, e.x7());                              \
                                                                               \
      construct_at<T8>::impl(p.x8##_ptr, e.x8());                              \
    }                                                                          \
                                                                               \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name NO_SPEC() const &inst) {          \
      _impl_expr(p, inst);                                                     \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            NS_PREFIX() name##_expr<E> const &e) {             \
      _impl_expr(p, e);                                                        \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p,         \
                            Args &&...args) {                                  \
      _impl_expr(p, NS_PREFIX() name NO_SPEC()(std::forward<Args>(args)...));  \
    }                                                                          \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct destroy_at<NS_PREFIX() name NO_SPEC()> {                              \
    static inline void impl(NS_PREFIX() name##_ptr NO_SPEC() const &p) {       \
                                                                               \
      destroy_at<T1>::impl(p.x1##_ptr);                                        \
                                                                               \
      destroy_at<T2>::impl(p.x2##_ptr);                                        \
                                                                               \
      destroy_at<T3>::impl(p.x3##_ptr);                                        \
                                                                               \
      destroy_at<T4>::impl(p.x4##_ptr);                                        \
                                                                               \
      destroy_at<T5>::impl(p.x5##_ptr);                                        \
                                                                               \
      destroy_at<T6>::impl(p.x6##_ptr);                                        \
                                                                               \
      destroy_at<T7>::impl(p.x7##_ptr);                                        \
                                                                               \
      destroy_at<T8>::impl(p.x8##_ptr);                                        \
    }                                                                          \
  };
