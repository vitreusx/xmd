

#pragma once
#include "allocator.h"
#include "const_ptr.h"
#include "const_ref.h"
#include "const_span.h"
#include "expr.h"
#include "inst.h"
#include "memory.h"
#include "meta.h"
#include "ptr.h"
#include "ref.h"
#include "span.h"
#include "vector.h"
#include <boost/serialization/access.hpp>
#include <type_traits>
#include <utility>

#define NAME() set_node
#define TEMPLATE_PARAMS(...) typename, T, __VA_ARGS__
#define FIELDS() T, value, bool, vacant

GEN_EXPR()
GEN_INST()
GEN_REF()
GEN_CONST_REF()
GEN_PTR()
GEN_CONST_PTR()
GEN_SPAN()
GEN_CONST_SPAN()
GEN_MEMORY()
GEN_ALLOCATOR()
GEN_VECTOR()

#undef NAME
#undef TEMPLATE_PARAMS
#undef FIELDS

template <typename T, typename Idx> struct set_iter {
  set_node_ref<T> ref;
  Idx idx;
};

template <typename T, typename NodeAlloc, typename Idx> class set_def {
public:
  inline explicit set_def(NodeAlloc const &alloc = NodeAlloc())
      : nodes{alloc} {};

  inline explicit set_def(Idx n, T const &init = T(),
                          NodeAlloc const &alloc = NodeAlloc())
      : nodes{n, set_node<T>(init, false), alloc} {};

  inline Idx size() const { return nodes.size(); }

  inline set_iter<T, Idx> insert(T const &value = T()) {
    Idx idx = nodes.size();
    return {nodes.emplace_back(value, false), idx};
  }

  template <typename... Args> inline set_iter<T, Idx> emplace(Args &&...args) {
    return insert(T(std::forward<Args>(args)...));
  }

  inline void remove(Idx idx) {
    destroy_at<T>::impl(&nodes[idx].value());
    nodes[idx].vacant() = true;
  }

  inline void clear() { nodes.clear(); }

  inline ref<set_node<T>> operator[](Idx idx) { return nodes[idx]; }

  inline ref<set_node<T>> at(Idx idx) { return nodes[idx]; }

  inline const_ref<set_node<T>> operator[](Idx idx) const { return nodes[idx]; }

  inline const_ref<set_node<T>> at(Idx idx) const { return nodes[idx]; }

protected:
  vector<set_node<T>, NodeAlloc, Idx> nodes;

private:
  friend class ::boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) {
    ar &nodes;
  }
};

template <typename T, typename NodeAlloc, typename Idx> struct set_impl {
  using type = set_def<T, NodeAlloc, Idx>;
};

template <typename T, typename NodeAlloc = allocator<set_node<T>>,
          typename Idx = int>
using set = typename set_impl<T, NodeAlloc, Idx>::type;

#define GEN_SET() GEN_SET_EXP(NAME())

#define GEN_SET_EXP(name) GEN_SET_(name)

#define GEN_SET_(name)                                                         \
                                                                               \
  ENTER_NS()                                                                   \
  TEMPLATE(typename, NodeAlloc, typename, Idx)                                 \
  class name##_set {                                                           \
  public:                                                                      \
    inline explicit name##_set(NodeAlloc const &alloc = NodeAlloc())           \
        : nodes{alloc} {};                                                     \
                                                                               \
    template <typename _U = name NO_SPEC()>                                    \
    inline explicit name##_set(                                                \
        Idx n, _U const &init = _U(), NodeAlloc const &alloc = NodeAlloc(),    \
        std::enable_if_t<std::is_default_constructible_v<_U>, _U *> = 0)       \
        : nodes{n, set_node<name NO_SPEC()>(init, false), alloc} {};           \
                                                                               \
    template <typename E>                                                      \
    inline name##_set(Idx n, name##_expr<E> const &e,                          \
                      NodeAlloc const &alloc = NodeAlloc())                    \
        : nodes{n, set_node<name NO_SPEC()>(e, false), alloc} {};              \
                                                                               \
    inline Idx extent() const { return nodes.size(); }                         \
                                                                               \
    template <typename E>                                                      \
    inline set_iter<name NO_SPEC(), Idx> insert(name##_expr<E> const &e) {     \
      Idx idx = nodes.size();                                                  \
      return {nodes.emplace_back(e, false), idx};                              \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    inline set_iter<name NO_SPEC(), Idx> emplace(Args &&...args) {             \
      return insert(name NO_SPEC()(std::forward<Args>(args)...));              \
    }                                                                          \
                                                                               \
    inline void remove(Idx idx) {                                              \
      destroy_at<name NO_SPEC()>::impl(ref_to_ptr(nodes[idx].value()));        \
      nodes[idx].vacant() = true;                                              \
    }                                                                          \
                                                                               \
    inline ref<set_node<name NO_SPEC()>> operator[](Idx idx) {                 \
      return nodes[idx];                                                       \
    }                                                                          \
                                                                               \
    inline ref<set_node<name NO_SPEC()>> at(Idx idx) { return nodes[idx]; }    \
                                                                               \
    inline const_ref<set_node<name NO_SPEC()>> operator[](Idx idx) const {     \
      return nodes[idx];                                                       \
    }                                                                          \
                                                                               \
    inline const_ref<set_node<name NO_SPEC()>> at(Idx idx) const {             \
      return nodes[idx];                                                       \
    }                                                                          \
                                                                               \
    inline void clear() { nodes.clear(); }                                     \
                                                                               \
  protected:                                                                   \
    vector<set_node<name NO_SPEC()>, NodeAlloc, Idx> nodes;                    \
                                                                               \
  private:                                                                     \
    friend class ::boost::serialization::access;                               \
                                                                               \
    template <class Archive>                                                   \
    void serialize(Archive &ar, [[maybe_unused]] const unsigned int version) { \
      ar &nodes;                                                               \
    }                                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  TEMPLATE(typename, NodeAlloc, typename, Idx)                                 \
  struct set_impl<NS_PREFIX() name NO_SPEC(), NodeAlloc, Idx> {                \
    using type = NS_PREFIX() name##_set                                        \
        SPEC(typename, NodeAlloc, typename, Idx);                              \
  };
