#pragma once
#include "span.h"
#include <xmd/utils/math.h>
#include <vector>
#include <iostream>

namespace xmd {
    template<typename T>
    class vector {
    public:
        vector(): data_{nullptr}, size_{0}, capacity_{0} {};

        explicit vector(int size, T const& init = T()) {
            size_ = capacity_ = size;

            data_ = (T *)malloc(size * sizeof(T));
            std::fill(data_, data_ + size_, init);
        }

        vector(vector const& other) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = (T *)malloc(size_ * sizeof(T));
            std::copy(other.data_, other.data_ + size_, data_);
        }

        vector& operator=(vector const& other) {
            this->~vector();
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = (T *)malloc(size_ * sizeof(T));
            std::copy(other.data_, other.data_ + size_, data_);
            return *this;
        }

        vector(vector&& other) {
            *this = std::move(other);
        }

        vector& operator=(vector&& other) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;

            other.size_ = other.capacity_ = 0;
            other.data_ = nullptr;
            return *this;
        }

        ~vector() {
            if (data_) {
                for (int idx = 0; idx < size_; ++idx) {
                    data_[idx].~T();
                }
                free(data_);
            }
            data_ = nullptr;
            size_ = capacity_ = 0;
        }

        int size() const {
            return size_;
        }

        T *data() const {
            return data_;
        }

        void reserve(int new_capacity) {
            T *new_data = (T *)malloc(new_capacity * sizeof(T));
            std::move(data_, data_ + size_, new_data);
            if (data_) free(data_);
            data_ = new_data;
            capacity_ = new_capacity;
        }

        void resize(int new_size, T const& init = T()) {
            if (new_size > capacity_) {
                int new_capacity = (int)ceil(1.5f * (float)new_size) + 32;
                reserve(new_capacity);
            }

            for (int idx = size_; idx < new_size; ++idx) {
                data_[idx] = init;
            }
            for (int idx = new_size; idx < size_; ++idx) {
                data_[idx].~T();
            }

            size_ = new_size;
        }

        template<typename... Args>
        T& emplace_back(Args&&... args) {
            if (size_ == capacity_) {
                int new_capacity = (int)ceil(1.5f * (float)capacity_) + 32;
                reserve(new_capacity);
            }

            return (data_[size_++] = T(std::forward<Args>(args)...));
        }

        T& push_back(T const& value = T()) {
             return emplace_back(value);
        }

        void clear() {
            size_ = 0;
        }

        T& operator[](int idx) {
            return data_[idx];
        }

        T const& operator[](int idx) const {
            return data_[idx];
        }

        T& at(int idx) {
            return (*this)[idx];
        }

        T const& at(int idx) const {
            return (*this)[idx];
        }

        span<T> to_span() {
            return span<T>(data_, size_);
        }

        span<const T> to_span() const {
            return span<T>(data_, size_);
        }

        array<T> to_array() {
            return data_;
        }

        array<const T> to_array() const {
            return data_;
        }

    private:
        T *data_;
        int size_, capacity_;
    };
}