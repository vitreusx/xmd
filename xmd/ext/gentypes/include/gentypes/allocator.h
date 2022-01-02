

#pragma once
#include "meta.h"
#include <memory>

template <typename T> struct allocator_impl { using type = std::allocator<T>; };

template <typename T> using allocator = typename allocator_impl<T>::type;

#define GEN_ALLOCATOR() GEN_ALLOCATOR_EXP(NAME(), FIELDS())

#define GEN_ALLOCATOR_EXP(name, fields) GEN_ALLOCATOR_(name, fields)

#define GEN_ALLOCATOR_(...) VFUNC(GEN_ALLOCATOR_, __VA_ARGS__)

#define GEN_ALLOCATOR_3(name, T1, x1)                                          \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_5(name, T1, x1, T2, x2)                                  \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_7(name, T1, x1, T2, x2, T3, x3)                          \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_9(name, T1, x1, T2, x2, T3, x3, T4, x4)                  \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      res.x4##_ptr = x4##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
                                                                               \
        x4##_alloc.deallocate(p.x4##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
                                                                               \
    allocator<T4> x4##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_11(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5)         \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      res.x4##_ptr = x4##_alloc.allocate(n);                                   \
                                                                               \
      res.x5##_ptr = x5##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
                                                                               \
        x4##_alloc.deallocate(p.x4##_ptr, n);                                  \
                                                                               \
        x5##_alloc.deallocate(p.x5##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
                                                                               \
    allocator<T4> x4##_alloc;                                                  \
                                                                               \
    allocator<T5> x5##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_13(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6) \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      res.x4##_ptr = x4##_alloc.allocate(n);                                   \
                                                                               \
      res.x5##_ptr = x5##_alloc.allocate(n);                                   \
                                                                               \
      res.x6##_ptr = x6##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
                                                                               \
        x4##_alloc.deallocate(p.x4##_ptr, n);                                  \
                                                                               \
        x5##_alloc.deallocate(p.x5##_ptr, n);                                  \
                                                                               \
        x6##_alloc.deallocate(p.x6##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
                                                                               \
    allocator<T4> x4##_alloc;                                                  \
                                                                               \
    allocator<T5> x5##_alloc;                                                  \
                                                                               \
    allocator<T6> x6##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_15(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, \
                         T7, x7)                                               \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      res.x4##_ptr = x4##_alloc.allocate(n);                                   \
                                                                               \
      res.x5##_ptr = x5##_alloc.allocate(n);                                   \
                                                                               \
      res.x6##_ptr = x6##_alloc.allocate(n);                                   \
                                                                               \
      res.x7##_ptr = x7##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
                                                                               \
        x4##_alloc.deallocate(p.x4##_ptr, n);                                  \
                                                                               \
        x5##_alloc.deallocate(p.x5##_ptr, n);                                  \
                                                                               \
        x6##_alloc.deallocate(p.x6##_ptr, n);                                  \
                                                                               \
        x7##_alloc.deallocate(p.x7##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
                                                                               \
    allocator<T4> x4##_alloc;                                                  \
                                                                               \
    allocator<T5> x5##_alloc;                                                  \
                                                                               \
    allocator<T6> x6##_alloc;                                                  \
                                                                               \
    allocator<T7> x7##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };

#define GEN_ALLOCATOR_17(name, T1, x1, T2, x2, T3, x3, T4, x4, T5, x5, T6, x6, \
                         T7, x7, T8, x8)                                       \
                                                                               \
  NO_TEMPLATE()                                                                \
  class name##_allocator {                                                     \
  public:                                                                      \
    name##_ptr NO_SPEC() allocate(size_t n) {                                  \
      name##_ptr NO_SPEC() res;                                                \
                                                                               \
      res.x1##_ptr = x1##_alloc.allocate(n);                                   \
                                                                               \
      res.x2##_ptr = x2##_alloc.allocate(n);                                   \
                                                                               \
      res.x3##_ptr = x3##_alloc.allocate(n);                                   \
                                                                               \
      res.x4##_ptr = x4##_alloc.allocate(n);                                   \
                                                                               \
      res.x5##_ptr = x5##_alloc.allocate(n);                                   \
                                                                               \
      res.x6##_ptr = x6##_alloc.allocate(n);                                   \
                                                                               \
      res.x7##_ptr = x7##_alloc.allocate(n);                                   \
                                                                               \
      res.x8##_ptr = x8##_alloc.allocate(n);                                   \
                                                                               \
      return res;                                                              \
    }                                                                          \
                                                                               \
    void deallocate(name##_ptr NO_SPEC() p, size_t n) {                        \
      if (p) {                                                                 \
                                                                               \
        x1##_alloc.deallocate(p.x1##_ptr, n);                                  \
                                                                               \
        x2##_alloc.deallocate(p.x2##_ptr, n);                                  \
                                                                               \
        x3##_alloc.deallocate(p.x3##_ptr, n);                                  \
                                                                               \
        x4##_alloc.deallocate(p.x4##_ptr, n);                                  \
                                                                               \
        x5##_alloc.deallocate(p.x5##_ptr, n);                                  \
                                                                               \
        x6##_alloc.deallocate(p.x6##_ptr, n);                                  \
                                                                               \
        x7##_alloc.deallocate(p.x7##_ptr, n);                                  \
                                                                               \
        x8##_alloc.deallocate(p.x8##_ptr, n);                                  \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    allocator<T1> x1##_alloc;                                                  \
                                                                               \
    allocator<T2> x2##_alloc;                                                  \
                                                                               \
    allocator<T3> x3##_alloc;                                                  \
                                                                               \
    allocator<T4> x4##_alloc;                                                  \
                                                                               \
    allocator<T5> x5##_alloc;                                                  \
                                                                               \
    allocator<T6> x6##_alloc;                                                  \
                                                                               \
    allocator<T7> x7##_alloc;                                                  \
                                                                               \
    allocator<T8> x8##_alloc;                                                  \
  };                                                                           \
                                                                               \
  NULL_SPEC()                                                                  \
  struct allocator_impl<name NO_SPEC()> {                                      \
    using type = name##_allocator NO_SPEC();                                   \
  };
