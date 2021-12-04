#pragma once
#include <xmd/math.h>
#include <xmd/types/vector.h>
#include <xmd/types/scalar.h>

namespace xmd {
    namespace v3 {
        template<typename E>
        class expr {
        public:
            auto x() const {
                return static_cast<E const&>(*this).x();
            }

            auto y() const {
                return static_cast<E const&>(*this).y();
            }

            auto z() const {
                return static_cast<E const&>(*this).z();
            }
        };

        template<typename U>
        class zero_expr: public expr<zero_expr<U>> {
        public:
            U x() const {
                return (U)0;
            }

            U y() const {
                return (U)0;
            }

            U z() const {
                return (U)0;
            }
        };

        template<typename U>
        class vec: public expr<vec<U>> {
        public:
            vec() = default;
            vec(U x, U y, U z):
                x_{x}, y_{y}, z_{z} {};

            template<typename E>
            vec(expr<E> const& e):
                x_{e.x()}, y_{e.y()}, z_{e.z()} {};

            static auto Zero() {
                return zero_expr<U>{};
            }

            template<typename E>
            auto& operator=(expr<E> const& e) {
                x() = e.x();
                y() = e.y();
                z() = e.z();
                return *this;
            }

            template<typename E2>
            auto& operator+=(expr<E2> const& e2) {
                x() += e2.x();
                y() += e2.y();
                z() += e2.z();
                return *this;
            }

            template<typename E2>
            auto& operator-=(expr<E2> const& e2) {
                x() -= e2.x();
                y() -= e2.y();
                z() -= e2.z();
                return *this;
            }

            template<typename S>
            auto& operator*=(S const& s) {
                x() *= s;
                y() *= s;
                z() *= s;
                return *this;
            }

            template<typename S>
            auto& operator/=(S const& s) {
                x() /= s;
                y() /= s;
                z() /= s;
                return *this;
            }

        public:
            U& x() {
                return x_;
            }

            U const& x() const {
                return x_;
            }

            U& y() {
                return y_;
            }

            U const& y() const {
                return y_;
            }

            U& z() {
                return z_;
            }

            U const& z() const {
                return z_;
            }

        private:
            U x_ = (U)0;
            U y_ = (U)0;
            U z_ = (U)0;
        };

        template<typename E>
        auto norm(v3::expr<E> const& e) {
            return xmd::sqrt(norm_squared(e));
        }

        template<typename E>
        auto norm_squared(v3::expr<E> const& e) {
            return dot(e, e);
        }

        template<typename E>
        auto l1_norm(v3::expr<E> const& e) {
            return xmd::abs(e.x()) + xmd::abs(e.y()) + xmd::abs(e.z());
        }

        template<typename E1, typename E2>
        auto dot(v3::expr<E1> const& e1, v3::expr<E2> const& e2) {
            return e1.x() * e2.x() + e1.y() * e2.y() + e1.z() * e2.z();
        }

        template<typename E>
        auto norm_inv(v3::expr<E> const& e) {
            return decltype(norm(e))(1.0) / norm(e);
        }

        template<typename E1, typename E2>
        class sum_expr: public expr<sum_expr<E1, E2>> {
        public:
            sum_expr(E1 const& e1, E2 const& e2):
                e1{e1}, e2{e2} {};

            auto x() const {
                return e1.x() + e2.x();
            }

            auto y() const {
                return e1.y() + e2.y();
            }

            auto z() const {
                return e1.z() + e2.z();
            }

        private:
            E1 e1;
            E2 e2;
        };

        template<typename E1, typename E2>
        auto operator+(expr<E1> const& e1, expr<E2> const& e2) {
            return sum_expr<E1, E2>(static_cast<E1 const&>(e1),
                static_cast<E2 const&>(e2));
        }

        template<typename E1, typename E2>
        class diff_expr: public expr<diff_expr<E1, E2>> {
        public:
            diff_expr(E1 const& e1, E2 const& e2):
                e1{e1}, e2{e2} {};

            auto x() const {
                return e1.x() - e2.x();
            }

            auto y() const {
                return e1.y() - e2.y();
            }

            auto z() const {
                return e1.z() - e2.z();
            }

        private:
            E1 e1;
            E2 e2;
        };

        template<typename E1, typename E2>
        auto operator-(expr<E1> const& e1, expr<E2> const& e2) {
            return diff_expr<E1, E2>(static_cast<E1 const&>(e1),
                static_cast<E2 const&>(e2));
        }

        template<typename E>
        class neg_expr: public expr<neg_expr<E>> {
        public:
            explicit neg_expr(E const& e):
                e{e} {};

            auto x() const {
                return -e.x();
            }

            auto y() const {
                return -e.y();
            }

            auto z() const {
                return -e.z();
            }

        private:
            E e;
        };

        template<typename E>
        auto operator-(expr<E> const& e) {
            return neg_expr<E>(static_cast<E const&>(e));
        }

        template<typename S, typename E>
        class scalar_lmul_expr: public expr<scalar_lmul_expr<S, E>> {
        public:
            explicit scalar_lmul_expr(S const& s, E const& e):
                s{s}, e{e} {};

            auto x() const {
                return s * e.x();
            }

            auto y() const {
                return s * e.y();
            }

            auto z() const {
                return s * e.z();
            }

        private:
            S s;
            E e;
        };

        template<typename S, typename E>
        auto operator*(S const& s, expr<E> const& e) {
            return scalar_lmul_expr<S, E>(s, static_cast<E const&>(e));
        }

        template<typename E, typename S>
        class scalar_rmul_expr: public expr<scalar_rmul_expr<E, S>> {
        public:
            explicit scalar_rmul_expr(E const& e, S const& s):
                e{e}, s{s} {};

            auto x() const {
                return e.x() * s;
            }

            auto y() const {
                return e.y() * s;
            }

            auto z() const {
                return e.z() * s;
            }

        private:
            E e;
            S s;
        };

        template<typename E, typename S>
        auto operator*(expr<E> const& e, S const& s) {
            return scalar_rmul_expr<E, S>(static_cast<E const&>(e), s);
        }

        template<typename E, typename S>
        class scalar_div_expr: public expr<scalar_div_expr<E, S>> {
        public:
            explicit scalar_div_expr(E const& e, S const& s):
                e{e}, s{s} {};

            auto x() const {
                return e.x() / s;
            }

            auto y() const {
                return e.y() / s;
            }

            auto z() const {
                return e.z() / s;
            }

        private:
            E e;
            S s;
        };

        template<typename E, typename S>
        auto operator/(expr<E> const& e, S const& s) {
            return scalar_div_expr<E, S>(static_cast<E const&>(e), s);
        }

        template<typename E1, typename E2>
        class cross_expr: public expr<cross_expr<E1, E2>> {
        public:
            cross_expr(E1 const &e1, E2 const &e2) :
                e1{e1}, e2{e2} {};

            auto x() const {
                return e1.y() * e2.z() - e1.z() * e2.y();
            }

            auto y() const {
                return e1.z() * e2.x() - e1.x() * e2.z();
            }

            auto z() const {
                return e1.x() * e2.y() - e1.y() * e2.x();
            }

        private:
            E1 e1;
            E2 e2;
        };

        template<typename E1, typename E2>
        auto cross(expr<E1> const& e1, expr<E2> const& e2) {
            return cross_expr<E1, E2>(static_cast<E1 const&>(e1),
                static_cast<E2 const&>(e2));
        }

        template<typename E>
        auto unit(expr<E> const& e) {
            return norm_inv(e) * e;
        }

        template<typename U>
        class at_expr: public expr<at_expr<U>> {
        public:
            at_expr(U* x, U* y, U* z, int idx):
                x_{x}, y_{y}, z_{z}, idx{idx} {};

            at_expr(at_expr<U> const& other):
                x_{other.x_}, y_{other.y_}, z_{other.z_}, idx{other.idx} {};

            __attribute__((always_inline)) auto& x() const {
                return x_[idx];
            }

            __attribute__((always_inline)) auto& y() const {
                return y_[idx];
            }

            __attribute__((always_inline)) auto& z() const {
                return z_[idx];
            }

            template<typename E>
            __attribute__((always_inline))
            void _assign_op_internal(U* __restrict__ xptr, U* __restrict__ yptr,
                U* __restrict__ zptr, expr<E> const& e) const {

                *xptr = e.x();
                *yptr = e.y();
                *zptr = e.z();
            }

            template<typename E>
            __attribute__((always_inline))
            auto& operator=(expr<E> const& e) const {
                _assign_op_internal(x_ + idx, y_ + idx, z_ + idx, e);
                return *this;
            }

            __attribute__((always_inline))
            auto& operator=(at_expr<U> const& e) const {
                _assign_op_internal(x_ + idx, y_ + idx, z_ + idx, e);
                return *this;
            }

            template<typename E>
            __attribute__((always_inline))
            void _add_op_internal(U* __restrict__ xptr, U* __restrict__ yptr,
                U* __restrict__ zptr, expr<E> const& e) const {

                *xptr += e.x();
                *yptr += e.y();
                *zptr += e.z();
            }

            template<typename E>
            __attribute__((always_inline))
            auto& operator+=(expr<E> const& e) const {
                _add_op_internal(x_ + idx, y_ + idx, z_ + idx, e);
                return *this;
            }

            template<typename E>
            __attribute__((always_inline))
            void _sub_op_internal(U* __restrict__ xptr, U* __restrict__ yptr,
                U* __restrict__ zptr, expr<E> const& e) const {

                *xptr -= e.x();
                *yptr -= e.y();
                *zptr -= e.z();
            }

            template<typename E>
            __attribute__((always_inline))
            auto& operator-=(expr<E> const& e) const {
                _sub_op_internal(x_ + idx, y_ + idx, z_ + idx, e);
                return *this;
            }

            template<typename S>
            __attribute__((always_inline))
            void _scalar_mul_op_internal(U* __restrict__ xptr, U* __restrict__ yptr,
                U* __restrict__ zptr, S const& s) const {

                *xptr *= s;
                *yptr *= s;
                *zptr *= s;
            }

            template<typename S>
            __attribute__((always_inline)) auto& operator*=(S const& s) const {
                _scalar_mul_op_internal(x_ + idx, y_ + idx, z_ + idx, s);
                return *this;
            }

            template<typename S>
            __attribute__((always_inline))
            void _scalar_div_op_internal(U* __restrict__ xptr, U* __restrict__ yptr,
                U* __restrict__ zptr, S const& s) const {

                *xptr /= s;
                *yptr /= s;
                *zptr /= s;
            }

            template<typename S>
            __attribute__((always_inline)) auto& operator/=(S const& s) const {
                _scalar_div_op_internal(x_ + idx, y_ + idx, z_ + idx, s);
                return *this;
            }

        private:
            U *x_, *y_, *z_;
            int idx;
        };

        template<typename U>
        class at_const_expr: public expr<at_const_expr<U>> {
        public:
            at_const_expr(U const* x, U const* y,
                U const* z, int idx):
                x_{x}, y_{y}, z_{z}, idx{idx} {};

            __attribute__((always_inline)) auto const& x() const {
                return x_[idx];
            }

            __attribute__((always_inline)) auto const& y() const {
                return y_[idx];
            }

            __attribute__((always_inline)) auto const& z() const {
                return z_[idx];
            }

        private:
            U const *x_, *y_, *z_;
            int idx;
        };

        template<typename U>
        class vec_array {
        public:
            vec_array(): x{nullptr}, y{nullptr}, z{nullptr} {};

            vec_array(U *x, U *y, U *z):
                x{x}, y{y}, z{z} {};

            auto operator[](int const& idx) const {
                return at_expr<U>(x, y, z, idx);
            }

        private:
            U *x, *y, *z;
        };

        template<typename U>
        class vec_span {
        public:
            vec_span(): x{nullptr}, y{nullptr}, z{nullptr}, size_{0} {};

            vec_span(U *x, U *y, U *z, int size):
                x{x}, y{y}, z{z}, size_{size} {};

            auto operator[](int const& idx) const {
                return at_expr<U>(x, y, z, idx);
            }

            int size() const {
                return size_;
            }

        private:
            U *x, *y, *z;
            int size_;
        };

        template<typename U>
        class vec_vector {
        public:
            vec_vector() = default;

            explicit vec_vector(int size):
                vec_vector(size, vec<U>()) {};

            template<typename E>
            explicit vec_vector(int size, expr<E> const& e):
                x{size, e.x()}, y{size, e.y()}, z{size, e.z()} {};

            int size() const {
                return x.size();
            }

            void reserve(int new_capacity) {
                x.reserve(new_capacity);
                y.reserve(new_capacity);
                z.reserve(new_capacity);
            }

            template<typename E>
            void resize(int new_size, expr<E> const& e = vec<U>()) {
                x.resize(new_size, e.x());
                y.resize(new_size, e.y());
                z.resize(new_size, e.z());
            }

            template<typename... Args>
            auto emplace_back(Args&&... args) {
                return push_back(vec<U>(std::forward<Args>(args)...));
            }

            template<typename E>
            auto push_back(expr<E> const& e) {
                x.push_back(e.x());
                y.push_back(e.y());
                z.push_back(e.z());
                return (*this)[size()-1];
            }

            void clear() {
                x.clear();
                y.clear();
                z.clear();
            }

            auto operator[](int const& idx) {
                return at_expr<U>(x.data(), y.data(), z.data(), idx);
            }

            auto operator[](int const& idx) const {
                return at_const_expr<U>(x.data(), y.data(), z.data(), idx);
            }

            vec_span<U> to_span() {
                return { x.data(), y.data(), z.data(), size() };
            }

            vec_span<U const> to_span() const {
                return { x.data(), y.data(), z.data(), size() };
            }

            vec_array<U> to_array() {
                return { x.data(), y.data(), z.data() };
            }

            vec_array<U const> to_array() const {
                return { x.data(), y.data(), z.data() };
            }

            vector<U> x, y, z;
        };
    }

    using vec3r = v3::vec<real>;
    using vec3r_array = v3::vec_array<real>;
    using vec3r_span = v3::vec_span<real>;
    using vec3r_vector = v3::vec_vector<real>;

    using vec3tr = v3::vec<true_real>;
    using vec3tr_array = v3::vec_array<true_real>;
    using vec3tr_span = v3::vec_span<true_real>;
    using vec3tr_vector = v3::vec_vector<true_real>;
}
