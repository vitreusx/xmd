#pragma once
#include "const_ptr.h"
#include "iterator.h"

template <typename T, typename Idx> class const_iterator {
public:
  inline explicit const_iterator(const_ptr<T> data, Idx idx)
      : data_{data}, idx_{idx} {};

  inline const_iterator(iterator<T, Idx> const &iter)
      : const_iterator{iter.data(), iter.idx()} {};

  const_ptr<T> data() const { return data_; }

  Idx idx() const { return idx_; }

  const_iterator &operator+=(Idx offset) {
    idx_ += offset;
    return *this;
  }

  const_iterator &operator++() {
    ++idx_;
    return *this;
  }

  const_iterator operator++(int) const { return const_iterator(data_, idx_++); }

  friend const_iterator operator+(const_iterator const &iter, Idx offset) {
    return iterator(iter.data_, iter.idx_ + offset);
  }

  friend const_iterator operator+(Idx offset, const_iterator const &iter) {
    return iterator(iter.data_, iter.idx_ + offset);
  }

  const_iterator &operator-=(Idx offset) {
    idx_ -= offset;
    return *this;
  }

  friend const_iterator operator-(const_iterator const &iter, Idx offset) {
    return iterator(iter.data_, iter.idx_ + offset);
  }

  friend Idx operator-(const_iterator const &iter1,
                       const_iterator const &iter2) {
    return iter1.idx_ - iter2.idx_;
  }

  const_iterator &operator--() {
    --idx_;
    return *this;
  }

  const_iterator operator--(int) const { return const_iterator(data_, idx_--); }

  const_ref<T> operator[](Idx idx) const { return data_[idx]; }

  const_ref<T> operator*() const { return data_[idx_]; }

  friend bool operator<(const_iterator const &iter1,
                        const_iterator const &iter2) {
    return iter1.idx_ < iter2.idx_;
  }

  friend bool operator>(const_iterator const &iter1,
                        const_iterator const &iter2) {
    return iter1.idx_ > iter2.idx_;
  }

  friend bool operator<=(const_iterator const &iter1,
                         const_iterator const &iter2) {
    return iter1.idx_ <= iter2.idx_;
  }

  friend bool operator>=(const_iterator const &iter1,
                         const_iterator const &iter2) {
    return iter1.idx_ >= iter2.idx_;
  }

  friend bool operator==(const_iterator const &iter1,
                         const_iterator const &iter2) {
    return iter1.idx_ == iter2.idx_;
  }

  friend bool operator!=(const_iterator const &iter1,
                         const_iterator const &iter2) {
    return iter1.idx_ != iter2.idx_;
  }

  typedef Idx difference_type;
  typedef T value_type;
  typedef const_ptr<T> pointer;
  typedef const_ref<T> reference;
  typedef std::random_access_iterator_tag iterator_category;

protected:
  const_ptr<T> data_;
  Idx idx_;
};