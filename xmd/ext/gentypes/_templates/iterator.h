#pragma once
#include <iterator>
#include "ref.h"
#include "ptr.h"

template<typename T, typename Idx>
class iterator {
public:
    inline explicit iterator(ptr<T> data, Idx idx):
        data_{data}, idx_{idx} {};
    
    ptr<T> data() const {
        return data_;
    }

    Idx idx() const {
        return idx_;
    }

    iterator& operator+=(Idx offset) {
        idx_ += offset;
        return *this;
    }

    iterator& operator++() {
        ++idx_;
        return *this;
    }

    iterator operator++(int) const {
        return iterator(data_, idx_++);
    }

    friend iterator operator+(iterator const& iter, Idx offset) {
        return iterator(iter.data_, iter.idx_ + offset);
    }
    
    friend iterator operator+(Idx offset, iterator const& iter) {
        return iterator(iter.data_, iter.idx_ + offset);
    }

    iterator& operator-=(Idx offset) {
        idx_ -= offset;
        return *this;
    }

    friend iterator operator-(iterator const& iter, Idx offset) {
        return iterator(iter.data_, iter.idx_ + offset);
    }

    friend Idx operator-(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ - iter2.idx_;
    }

    iterator& operator--() {
        --idx_;
        return *this;
    }

    iterator operator--(int) const {
        return iterator(data_, idx_--);
    }

    ref<T> operator[](Idx idx) const {
        return data_[idx];
    }

    ref<T> operator*() const {
        return data_[idx_];
    }

    friend bool operator<(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ < iter2.idx_;
    }

    friend bool operator>(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ > iter2.idx_;
    }

    friend bool operator<=(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ <= iter2.idx_;
    }

    friend bool operator>=(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ >= iter2.idx_;
    }

    friend bool operator==(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ == iter2.idx_;
    }

    friend bool operator!=(iterator const& iter1, iterator const& iter2) {
        return iter1.idx_ != iter2.idx_;
    }

    typedef Idx difference_type;
    typedef T value_type;
    typedef ptr<T> pointer;
    typedef ref<T> reference;
    typedef std::random_access_iterator_tag iterator_category;

protected:
    ptr<T> data_;
    Idx idx_;
};

