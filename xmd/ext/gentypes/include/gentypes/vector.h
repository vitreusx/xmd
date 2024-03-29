

#pragma once
#include "const_iterator.h"
#include "iterator.h"
#include "memory.h"
#include "meta.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <type_traits>
#include <vector>

template <typename T, typename Alloc, typename Idx> class vector_def {
public:
  inline explicit vector_def(Alloc const &alloc = Alloc())
      : data_{nullptr}, size_{0}, capacity_{0}, alloc_{alloc} {};

  inline vector_def(Idx n, T const &init = T(), Alloc const &alloc = Alloc()) {
    alloc_ = alloc;
    size_ = capacity_ = n;

    data_ = alloc_.allocate(size_);
    uninitialized_fill_n<T>::impl(data_, n, init);
  }

  inline ~vector_def() {
    if (data_) {
      destroy_n<T>::impl(data_, size_);
      alloc_.deallocate(data_, capacity_);
    }
    data_ = nullptr;
    size_ = capacity_ = 0;
    alloc_.~Alloc();
  }

  inline vector_def(vector_def const &other) {
    alloc_ = other.alloc_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    data_ = alloc_.allocate(capacity_);
    uninitialized_copy_n<T>::impl(other.data_, size_, data_);
  }

  inline vector_def &operator=(vector_def const &other) {
    this->~vector_def();

    alloc_ = other.alloc_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    data_ = alloc_.allocate(capacity_);
    uninitialized_copy_n<T>::impl(other.data_, size_, data_);

    return *this;
  }

  inline vector_def(vector_def &&other) {
    data_ = std::move(other.data_);
    other.data_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    alloc_ = std::move(other.alloc_);
  }

  inline vector_def &operator=(vector_def &&other) {
    this->~vector_def();

    data_ = std::move(other.data_);
    other.data_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    alloc_ = std::move(other.alloc_);

    return *this;
  }

  inline Idx size() const { return size_; }

  inline Idx capacity() const { return capacity_; }

  inline T *data() { return data_; }

  inline operator T *() { return data(); }

  inline T const *data() const { return data_; }

  inline operator T const *() const { return data(); }

  inline span<T, Idx> view() { return span<T, Idx>(data_, size_); }

  inline operator span<T, Idx>() { return view(); }

  inline const_span<T, Idx> view() const {
    return const_span<T, Idx>(data_, size_);
  }

  inline operator const_span<T, Idx>() const { return view(); }

  inline T &operator[](Idx idx) { return data_[idx]; }

  inline T &at(Idx idx) { return data_[idx]; }

  inline T const &operator[](Idx idx) const { return data_[idx]; }

  inline T const &at(Idx idx) const { return data_[idx]; }

  inline void push_back(T const &value) { emplace_back(T(value)); }

  template <typename... Args> inline T &emplace_back(Args &&...args) {
    if (size_ >= capacity_) {
      Idx new_capacity = 2 * capacity_ + 8;
      reserve(new_capacity);
    }

    uninitialized_fill_n<T>::impl(data_ + size_, 1,
                                  T(std::forward<Args>(args)...));
    return data_[size_++];
  }

  inline void reserve(Idx new_capacity) {
    if (capacity_ < new_capacity) {
      T *new_data = alloc_.allocate(new_capacity);
      uninitialized_move_n<T>::impl(data_, size_, new_data);
      alloc_.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

  inline void resize(Idx new_size, T const &fill = T()) {
    if (new_size < size_) {
      destroy_n<T>::impl(data_ + new_size, size_ - new_size);
    } else if (size_ < new_size) {
      reserve(new_size);
      uninitialized_fill_n<T>::impl(data_ + size_, new_size - size_, fill);
    }
    size_ = new_size;
  }

  inline void clear() {
    destroy_n<T>::impl(data_, size_);
    size_ = 0;
  }

  inline iterator<T, Idx> begin() { return iterator<T, Idx>(data_, 0); }

  inline const_iterator<T, Idx> begin() const {
    return const_iterator<T, Idx>(data_, 0);
  }

  inline iterator<T, Idx> end() { return iterator<T, Idx>(data_, size_); }

  inline const_iterator<T, Idx> end() const {
    return const_iterator<T, Idx>(data_, size_);
  }

protected:
  T *data_;
  Idx size_, capacity_;
  Alloc alloc_;

private:
  friend class ::boost::serialization::access;

  template <class Archive>
  void save(Archive &ar, [[maybe_unused]] const unsigned int version) const {
    std::vector<T> V;
    for (Idx idx = 0; idx < size_; ++idx) {
      V.push_back(at(idx));
    }

    ar &V;
  }

  template <class Archive>
  void load(Archive &ar, [[maybe_unused]] const unsigned int version) {
    std::vector<T> V;
    ar &V;

    clear();
    for (Idx idx = 0; idx < (Idx)V.size(); ++idx) {
      push_back(V[idx]);
    }
  }

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ::boost::serialization::split_member(ar, *this, version);
  }
};

template <typename T, typename Alloc, typename Idx> struct vector_impl {
  using type = vector_def<T, Alloc, Idx>;
};

template <typename T, typename Alloc = allocator<T>, typename Idx = int>
using vector = typename vector_impl<T, Alloc, Idx>::type;

#define GEN_VECTOR() GEN_VECTOR_EXP(NAME())

#define GEN_VECTOR_EXP(name) GEN_VECTOR_(name)

#define GEN_VECTOR_(name)                                                      \
                                                                               \
  ENTER_NS()                                                                   \
  TEMPLATE(typename, Alloc, typename, Idx)                                     \
  class name##_vector {                                                        \
  public:                                                                      \
    inline explicit name##_vector(Alloc const &alloc = Alloc())                \
        : data_{nullptr}, size_{0}, capacity_{0}, alloc_{alloc} {};            \
                                                                               \
    template <typename _U = name NO_SPEC()>                                    \
    inline name##_vector(                                                      \
        Idx n, _U const &init = _U(), Alloc const &alloc = Alloc(),            \
        std::enable_if_t<std::is_default_constructible_v<_U>, _U *> = 0) {     \
      alloc_ = alloc;                                                          \
      size_ = capacity_ = n;                                                   \
                                                                               \
      data_ = alloc_.allocate(size_);                                          \
      uninitialized_fill_n<name NO_SPEC()>::impl(data_, n, init);              \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    inline name##_vector(Idx n, name##_expr<E> const &e,                       \
                         Alloc const &alloc = Alloc()) {                       \
      alloc_ = alloc;                                                          \
      size_ = capacity_ = n;                                                   \
                                                                               \
      data_ = alloc_.allocate(size_);                                          \
      uninitialized_fill_n<name NO_SPEC()>::impl(data_, n, e);                 \
    }                                                                          \
                                                                               \
    inline ~name##_vector() {                                                  \
      if (data_) {                                                             \
        destroy_n<name NO_SPEC()>::impl(data_, size_);                         \
        alloc_.deallocate(data_, capacity_);                                   \
      }                                                                        \
      data_ = nullptr;                                                         \
      size_ = capacity_ = 0;                                                   \
      alloc_.~Alloc();                                                         \
    }                                                                          \
                                                                               \
    inline name##_vector(name##_vector const &other) {                         \
      alloc_ = other.alloc_;                                                   \
      size_ = other.size_;                                                     \
      capacity_ = other.capacity_;                                             \
                                                                               \
      data_ = alloc_.allocate(capacity_);                                      \
      uninitialized_copy_n<name NO_SPEC()>::impl(other.data_, size_, data_);   \
    }                                                                          \
                                                                               \
    inline name##_vector &operator=(name##_vector const &other) {              \
      this->~name##_vector();                                                  \
                                                                               \
      alloc_ = other.alloc;                                                    \
      size_ = other.size_;                                                     \
      capacity_ = other.capacity_;                                             \
                                                                               \
      data_ = alloc_.allocate(capacity_);                                      \
      uninitialized_copy_n<name NO_SPEC()>::impl(other.data_, size_, data_);   \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline name##_vector(name##_vector &&other) {                              \
      data_ = std::move(other.data_);                                          \
      other.data_ = nullptr;                                                   \
      size_ = other.size_;                                                     \
      other.size_ = 0;                                                         \
      capacity_ = other.capacity_;                                             \
      other.capacity_ = 0;                                                     \
      alloc_ = std::move(other.alloc_);                                        \
    }                                                                          \
                                                                               \
    inline name##_vector &operator=(name##_vector &&other) {                   \
      this->~name##_vector();                                                  \
                                                                               \
      data_ = std::move(other.data_);                                          \
      other.data_ = nullptr;                                                   \
      size_ = other.size_;                                                     \
      other.size_ = 0;                                                         \
      capacity_ = other.capacity_;                                             \
      other.capacity_ = 0;                                                     \
      alloc_ = std::move(other.alloc_);                                        \
                                                                               \
      return *this;                                                            \
    }                                                                          \
                                                                               \
    inline Idx size() const { return size_; }                                  \
                                                                               \
    inline Idx capacity() const { return capacity_; }                          \
                                                                               \
    inline name##_ptr NO_SPEC() data() { return data_; }                       \
                                                                               \
    inline operator name##_ptr NO_SPEC()() { return data(); }                  \
                                                                               \
    inline name##_const_ptr NO_SPEC() data() const { return data_; }           \
                                                                               \
    inline operator name##_const_ptr NO_SPEC()() const { return data(); }      \
                                                                               \
    inline name##_span SPEC(typename, Idx) view() {                            \
      return name##_span SPEC(typename, Idx)(data_, size_);                    \
    }                                                                          \
                                                                               \
    inline operator name##_span SPEC(typename, Idx)() { return view(); }       \
                                                                               \
    inline name##_const_span SPEC(typename, Idx) view() const {                \
      return name##_span SPEC(typename, Idx)(data_, size_);                    \
    }                                                                          \
                                                                               \
    inline operator name##_const_span SPEC(typename, Idx)() const {            \
      return view();                                                           \
    }                                                                          \
                                                                               \
    inline name##_ref NO_SPEC() operator[](Idx idx) { return data_[idx]; }     \
                                                                               \
    inline name##_ref NO_SPEC() at(Idx idx) { return data_[idx]; }             \
                                                                               \
    inline name##_const_ref NO_SPEC() operator[](Idx idx) const {              \
      return data_[idx];                                                       \
    }                                                                          \
                                                                               \
    inline name##_const_ref NO_SPEC() at(Idx idx) const { return data_[idx]; } \
                                                                               \
    template <typename E> inline void push_back(name##_expr<E> const &e) {     \
      if (size_ >= capacity_) {                                                \
        Idx new_capacity = 2 * capacity_ + 8;                                  \
        reserve(new_capacity);                                                 \
      }                                                                        \
                                                                               \
      construct_at<name NO_SPEC()>::impl(data_ + size_, e);                    \
    }                                                                          \
                                                                               \
    template <typename... Args>                                                \
    inline name##_ref NO_SPEC() emplace_back(Args &&...args) {                 \
      if (size_ >= capacity_) {                                                \
        Idx new_capacity = 2 * capacity_ + 8;                                  \
        reserve(new_capacity);                                                 \
      }                                                                        \
                                                                               \
      construct_at<name NO_SPEC()>::impl(data_ + size_,                        \
                                         std::forward<Args>(args)...);         \
      return data_[size_++];                                                   \
    }                                                                          \
                                                                               \
    inline void reserve(Idx new_capacity) {                                    \
      if (capacity_ < new_capacity) {                                          \
        name##_ptr NO_SPEC() new_data = alloc_.allocate(new_capacity);         \
        uninitialized_move_n<name NO_SPEC()>::impl(data_, size_, new_data);    \
        alloc_.deallocate(data_, capacity_);                                   \
        data_ = new_data;                                                      \
        capacity_ = new_capacity;                                              \
      }                                                                        \
    }                                                                          \
                                                                               \
    template <typename _U = name NO_SPEC()>                                    \
    inline void                                                                \
    resize(Idx new_size, _U const &fill = _U(),                                \
           std::enable_if_t<std::is_default_constructible_v<_U>, _U *> = 0) {  \
      if (new_size < size_) {                                                  \
        destroy_n<name NO_SPEC()>::impl(data_ + new_size, size_ - new_size);   \
      } else if (size_ < new_size) {                                           \
        reserve(new_size);                                                     \
        uninitialized_fill_n<name NO_SPEC()>::impl(data_ + size_,              \
                                                   new_size - size_, fill);    \
      }                                                                        \
      size_ = new_size;                                                        \
    }                                                                          \
                                                                               \
    template <typename E>                                                      \
    inline void resize(Idx new_size, name##_expr<E> const &e) {                \
      if (new_size < size_) {                                                  \
        destroy_n<name NO_SPEC()>::impl(data_ + new_size, size_ - new_size);   \
      } else if (size_ < new_size) {                                           \
        reserve(new_size);                                                     \
        uninitialized_fill_n<name NO_SPEC()>::impl(data_ + size_,              \
                                                   new_size - size_, e);       \
      }                                                                        \
      size_ = new_size;                                                        \
    }                                                                          \
                                                                               \
    inline void clear() {                                                      \
      destroy_n<name NO_SPEC()>::impl(data_, size_);                           \
      size_ = 0;                                                               \
    }                                                                          \
                                                                               \
    inline iterator<name NO_SPEC(), Idx> begin() {                             \
      return iterator<name NO_SPEC(), Idx>(data_, 0);                          \
    }                                                                          \
                                                                               \
    inline const_iterator<name NO_SPEC(), Idx> begin() const {                 \
      return const_iterator<name NO_SPEC(), Idx>(data_, 0);                    \
    }                                                                          \
                                                                               \
    inline iterator<name NO_SPEC(), Idx> end() {                               \
      return iterator<name NO_SPEC(), Idx>(data_, size_);                      \
    }                                                                          \
                                                                               \
    inline const_iterator<name NO_SPEC(), Idx> end() const {                   \
      return const_iterator<name NO_SPEC(), Idx>(data_, size_);                \
    }                                                                          \
                                                                               \
    inline bool empty() const { return size_ == 0; }                           \
                                                                               \
  protected:                                                                   \
    name##_ptr NO_SPEC() data_;                                                \
    Idx size_, capacity_;                                                      \
    Alloc alloc_;                                                              \
                                                                               \
  private:                                                                     \
    friend class ::boost::serialization::access;                               \
                                                                               \
    template <class Archive>                                                   \
    void save(Archive &ar,                                                     \
              [[maybe_unused]] const unsigned int version) const {             \
      std::vector<name NO_SPEC()> V;                                           \
      for (Idx idx = 0; idx < size_; ++idx) {                                  \
        name NO_SPEC() item = at(idx);                                         \
        V.push_back(item);                                                     \
      }                                                                        \
                                                                               \
      ar &V;                                                                   \
    }                                                                          \
                                                                               \
    template <class Archive>                                                   \
    void load(Archive &ar, [[maybe_unused]] const unsigned int version) {      \
      std::vector<name NO_SPEC()> V;                                           \
      ar &V;                                                                   \
                                                                               \
      clear();                                                                 \
      for (Idx idx = 0; idx < (Idx)V.size(); ++idx) {                          \
        push_back(V[idx]);                                                     \
      }                                                                        \
    }                                                                          \
                                                                               \
    template <class Archive>                                                   \
    void serialize(Archive &ar, const unsigned int version) {                  \
      ::boost::serialization::split_member(ar, *this, version);                \
    }                                                                          \
  };                                                                           \
  LEAVE_NS()                                                                   \
                                                                               \
  TEMPLATE(typename, Alloc, typename, Idx)                                     \
  struct vector_impl<NS_PREFIX() name NO_SPEC(), Alloc, Idx> {                 \
    using type = NS_PREFIX() name##_vector                                     \
        SPEC(typename, Alloc, typename, Idx);                                  \
  };
