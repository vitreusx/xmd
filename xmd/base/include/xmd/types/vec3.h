#pragma once
#include <xmd/math.h>

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
        class vector: public expr<vector<U>> {
        public:
            vector() = default;
            vector(U x, U y, U z):
                x_{x}, y_{y}, z_{z} {};

            template<typename E>
            vector(expr<E> const& e):
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
        auto norm(expr<E> const& e) {
            return xmd::sqrt(norm_squared(e));
        }

        template<typename E>
        auto norm_squared(expr<E> const& e) {
            return dot(e, e);
        }

        template<typename E>
        auto l1_norm(expr<E> const& e) {
            return xmd::abs(e.x()) + xmd::abs(e.y()) + xmd::abs(e.z());
        }

        template<typename E1, typename E2>
        auto dot(expr<E1> const& e1, expr<E2> const& e2) {
            return e1.x() * e2.x() + e1.y() * e2.y() + e1.z() * e2.z();
        }

        template<typename E>
        auto norm_inv(expr<E> const& e) {
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
            E1 const& e1;
            E2 const& e2;
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
            E1 const& e1;
            E2 const& e2;
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
            E const& e;
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
            S const& s;
            E const& e;
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
            E const& e;
            S const& s;
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
            E const& e;
            S const& s;
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
            E1 const& e1;
            E2 const& e2;
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
            at_expr(U* const& x, U* const& y, U* const& z, int const& idx):
                x_{x}, y_{y}, z_{z}, idx{idx} {};

            auto& x() const {
                return x_[idx];
            }

            auto& y() const {
                return y_[idx];
            }

            auto& z() const {
                return z_[idx];
            }

            template<typename E>
            auto& operator=(expr<E> const& e) const {
                ([&](float* __restrict__ xptr, float* __restrict__ yptr,
                    float* __restrict__ zptr) -> void {

                    *xptr = e.x();
                    *yptr = e.y();
                    *zptr = e.z();
                })(x_ + idx, y_ + idx, z_ + idx);

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

        private:
            U* const& x_;
            U* const& y_;
            U* const& z_;
            int const& idx;
        };

        template<typename U>
        class at_const_expr: public expr<at_const_expr<U>> {
        public:
            at_const_expr(U const* const& x, U const* const& y,
                U const* const& z, int const& idx):
                x_{x}, y_{y}, z_{z}, idx{idx} {};

            auto const& x() const {
                return x_[idx];
            }

            auto const& y() const {
                return y_[idx];
            }

            auto const& z() const {
                return z_[idx];
            }

        private:
            U const* const& x_;
            U const* const& y_;
            U const* const& z_;
            int const& idx;
        };

        template<typename U>
        class vector_array {
        public:
            vector_array(U *x, U *y, U *z, int size):
                x{x}, y{y}, z{z}, size{size} {};

            auto operator[](int const& idx) {
                return at_expr<U>(x, y, z, idx);
            }

            auto operator[](int const& idx) const {
                return at_const_expr<U>(x, y, z, idx);
            }

        private:
            U *x, *y, *z;
            int size;
        };
    }

    using vec3f = v3::vector<float>;
    using vec3f_array = v3::vector_array<float>;
}
