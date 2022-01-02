

#pragma once
#include "meta.h"
#include <memory>

template <typename T>
void uninitialized_fill_n(T *data, size_t n, T const &init) {
  std::uninitialized_fill_n(data, n, init);
}

template <typename T>
void uninitialized_copy_n(T const *from, size_t n, T *to) {
  std::uninitialized_copy_n(from, n, to);
}

template <typename T> void uninitialized_move_n(T *from, size_t n, T *to) {
  std::uninitialized_move_n(from, n, to);
}

template <typename T> void destroy_n(T *data, size_t n) {
  std::destroy_n(data, n);
}

template <typename T, typename... Args>
void construct_at(T *p, Args &&...args) {
  ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
}

template <typename T> void destroy_at(T *p) { std::destroy_at(p); }

#define GEN_MEMORY() GEN_MEMORY_EXP(NAME(), FIELDS())

#define GEN_MEMORY_EXP(name, fields) GEN_MEMORY_(name, fields)

#define GEN_MEMORY_(...) VFUNC(GEN_MEMORY_, __VA_ARGS__)

#define GEN_MEMORY_3(name, T1, x1)                                             \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
  }

#define GEN_MEMORY_5(name, T1, x1, T2, x2)                                     \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
  }

#define GEN_MEMORY_7(name, T1, x1, T2, x2, T3, x3)                             \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
  }

#define GEN_MEMORY_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                     \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
                                                                               \
    uninitialized_fill_n(p.x4##_ptr, n, e.x4());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x4##_ptr, n, to.x4##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x4##_ptr, n, to.x4##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x4##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
                                                                               \
    construct_at(p.x4##_ptr, e.x4());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
                                                                               \
    destroy_at(p.x4##_ptr);                                                    \
  }

#define GEN_MEMORY_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
                                                                               \
    uninitialized_fill_n(p.x4##_ptr, n, e.x4());                               \
                                                                               \
    uninitialized_fill_n(p.x5##_ptr, n, e.x5());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x5##_ptr, n, to.x5##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x5##_ptr, n, to.x5##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x4##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x5##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
                                                                               \
    construct_at(p.x4##_ptr, e.x4());                                          \
                                                                               \
    construct_at(p.x5##_ptr, e.x5());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
                                                                               \
    destroy_at(p.x4##_ptr);                                                    \
                                                                               \
    destroy_at(p.x5##_ptr);                                                    \
  }

#define GEN_MEMORY_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6)    \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
                                                                               \
    uninitialized_fill_n(p.x4##_ptr, n, e.x4());                               \
                                                                               \
    uninitialized_fill_n(p.x5##_ptr, n, e.x5());                               \
                                                                               \
    uninitialized_fill_n(p.x6##_ptr, n, e.x6());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x6##_ptr, n, to.x6##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x6##_ptr, n, to.x6##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x4##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x5##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x6##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
                                                                               \
    construct_at(p.x4##_ptr, e.x4());                                          \
                                                                               \
    construct_at(p.x5##_ptr, e.x5());                                          \
                                                                               \
    construct_at(p.x6##_ptr, e.x6());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
                                                                               \
    destroy_at(p.x4##_ptr);                                                    \
                                                                               \
    destroy_at(p.x5##_ptr);                                                    \
                                                                               \
    destroy_at(p.x6##_ptr);                                                    \
  }

#define GEN_MEMORY_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7)                                                  \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
                                                                               \
    uninitialized_fill_n(p.x4##_ptr, n, e.x4());                               \
                                                                               \
    uninitialized_fill_n(p.x5##_ptr, n, e.x5());                               \
                                                                               \
    uninitialized_fill_n(p.x6##_ptr, n, e.x6());                               \
                                                                               \
    uninitialized_fill_n(p.x7##_ptr, n, e.x7());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x6##_ptr, n, to.x6##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x7##_ptr, n, to.x7##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x6##_ptr, n, to.x6##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x7##_ptr, n, to.x7##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x4##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x5##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x6##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x7##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
                                                                               \
    construct_at(p.x4##_ptr, e.x4());                                          \
                                                                               \
    construct_at(p.x5##_ptr, e.x5());                                          \
                                                                               \
    construct_at(p.x6##_ptr, e.x6());                                          \
                                                                               \
    construct_at(p.x7##_ptr, e.x7());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
                                                                               \
    destroy_at(p.x4##_ptr);                                                    \
                                                                               \
    destroy_at(p.x5##_ptr);                                                    \
                                                                               \
    destroy_at(p.x6##_ptr);                                                    \
                                                                               \
    destroy_at(p.x7##_ptr);                                                    \
  }

#define GEN_MEMORY_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6,    \
                      T7, x7, T8, x8)                                          \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void uninitialized_fill_n(name##_ptr NO_SPEC() const &p, size_t n,    \
                                   name##_expr<E> const &e) {                  \
                                                                               \
    uninitialized_fill_n(p.x1##_ptr, n, e.x1());                               \
                                                                               \
    uninitialized_fill_n(p.x2##_ptr, n, e.x2());                               \
                                                                               \
    uninitialized_fill_n(p.x3##_ptr, n, e.x3());                               \
                                                                               \
    uninitialized_fill_n(p.x4##_ptr, n, e.x4());                               \
                                                                               \
    uninitialized_fill_n(p.x5##_ptr, n, e.x5());                               \
                                                                               \
    uninitialized_fill_n(p.x6##_ptr, n, e.x6());                               \
                                                                               \
    uninitialized_fill_n(p.x7##_ptr, n, e.x7());                               \
                                                                               \
    uninitialized_fill_n(p.x8##_ptr, n, e.x8());                               \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_copy_n(name##_const_ptr NO_SPEC() const &from,     \
                                   size_t n, name##_ptr NO_SPEC() const &to) { \
                                                                               \
    uninitialized_copy_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x6##_ptr, n, to.x6##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x7##_ptr, n, to.x7##_ptr);                       \
                                                                               \
    uninitialized_copy_n(from.x8##_ptr, n, to.x8##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void uninitialized_move_n(name##_ptr NO_SPEC() const &from, size_t n, \
                                   name##_ptr NO_SPEC() const &to) {           \
                                                                               \
    uninitialized_move_n(from.x1##_ptr, n, to.x1##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x2##_ptr, n, to.x2##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x3##_ptr, n, to.x3##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x4##_ptr, n, to.x4##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x5##_ptr, n, to.x5##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x6##_ptr, n, to.x6##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x7##_ptr, n, to.x7##_ptr);                       \
                                                                               \
    uninitialized_move_n(from.x8##_ptr, n, to.x8##_ptr);                       \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_n(name##_ptr NO_SPEC() const &p, size_t n) {             \
                                                                               \
    destroy_n(p.x1##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x2##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x3##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x4##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x5##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x6##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x7##_ptr, n);                                                  \
                                                                               \
    destroy_n(p.x8##_ptr, n);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void _construct_at_expr(name##_ptr NO_SPEC() const &p,                \
                                 name##_expr<E> const &e) {                    \
                                                                               \
    construct_at(p.x1##_ptr, e.x1());                                          \
                                                                               \
    construct_at(p.x2##_ptr, e.x2());                                          \
                                                                               \
    construct_at(p.x3##_ptr, e.x3());                                          \
                                                                               \
    construct_at(p.x4##_ptr, e.x4());                                          \
                                                                               \
    construct_at(p.x5##_ptr, e.x5());                                          \
                                                                               \
    construct_at(p.x6##_ptr, e.x6());                                          \
                                                                               \
    construct_at(p.x7##_ptr, e.x7());                                          \
                                                                               \
    construct_at(p.x8##_ptr, e.x8());                                          \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name NO_SPEC() const &inst) {                       \
    _construct_at_expr(p, inst);                                               \
  }                                                                            \
                                                                               \
  TEMPLATE(typename, E)                                                        \
  inline void construct_at(name##_ptr NO_SPEC() const &p,                      \
                           name##_expr<E> const &e) {                          \
    _construct_at_expr(p, e);                                                  \
  }                                                                            \
                                                                               \
  TEMPLATE(typename..., Args)                                                  \
  inline void construct_at(name##_ptr NO_SPEC() const &p, Args &&...args) {    \
    _construct_at_expr(p, name NO_SPEC()(std::forward<Args>(args)...));        \
  }                                                                            \
                                                                               \
  NO_TEMPLATE()                                                                \
  inline void destroy_at(name##_ptr NO_SPEC() const &p) {                      \
                                                                               \
    destroy_at(p.x1##_ptr);                                                    \
                                                                               \
    destroy_at(p.x2##_ptr);                                                    \
                                                                               \
    destroy_at(p.x3##_ptr);                                                    \
                                                                               \
    destroy_at(p.x4##_ptr);                                                    \
                                                                               \
    destroy_at(p.x5##_ptr);                                                    \
                                                                               \
    destroy_at(p.x6##_ptr);                                                    \
                                                                               \
    destroy_at(p.x7##_ptr);                                                    \
                                                                               \
    destroy_at(p.x8##_ptr);                                                    \
  }
