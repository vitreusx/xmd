#pragma once
#include <gentypes/gentype.h>
#include <xmd/utils/math.h>
#include <xmd/types/scalar.h>
#include <yaml-cpp/yaml.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define NAME() vec3
#define TEMPLATE_PARAMS(...) typename,U,__VA_ARGS__
#define FIELDS() U,x,U,y,U,z

GEN_EXPR()

namespace xmd {
    template<typename U>
    class zero_expr : public vec3_expr<zero_expr<U>> {
    public:
        U x() const {
            return 0;
        }

        U y() const {
            return 0;
        }

        U z() const {
            return 0;
        }
    };

    template<typename U>
    class vec3 : public vec3_expr<vec3<U>> {
    public:
        vec3() :
            x_{0}, y_{0}, z_{0} {};

        INST_CTORS()
        INST_ASSIGN_COPY()
        INST_ASSIGN_EXPR()

        static auto Zero() {
            return zero_expr<U>{};
        }

        template<typename E>
        auto &operator+=(vec3_expr<E> const &e) {
            x_ += e.x();
            y_ += e.y();
            z_ += e.z();
            return *this;
        }

        template<typename E>
        auto &operator-=(vec3_expr<E> const &e) {
            x_ -= e.x();
            y_ -= e.y();
            z_ -= e.z();
            return *this;
        }

        template<typename S>
        auto &operator*=(S const &s) {
            x_ *= s;
            y_ *= s;
            z_ *= s;
            return *this;
        }

        template<typename S>
        auto &operator/=(S const &s) {
            x_ /= s;
            y_ /= s;
            z_ /= s;
            return *this;
        }

        INST_LAZY_FIELDS()

    private:
        INST_FIELDS()
    };

    template<typename U>
    class vec3_ref : public vec3_expr<vec3_ref<U>> {
    public:
        REF_CTORS()
        REF_ASSIGN_COPY()
        REF_ASSIGN_MOVE()
        REF_ASSIGN_EXPR()
        REF_SWAP()
        REF_LAZY_FIELDS()

        template<typename E>
        auto &operator+=(vec3_expr<E> const &e) {
            x_ += e.x();
            y_ += e.y();
            z_ += e.z();
            return *this;
        }

        template<typename E>
        auto &atomic_add(vec3_expr<E> const &e) {
#pragma omp atomic update
            x_ += e.x();
#pragma omp atomic update
            y_ += e.y();
#pragma omp atomic update
            z_ += e.z();
            return *this;
        }

        template<typename E>
        auto &operator-=(vec3_expr<E> const &e) {
            x_ -= e.x();
            y_ -= e.y();
            z_ -= e.z();
            return *this;
        }

        template<typename S>
        auto &operator*=(S const &s) {
            x_ *= s;
            y_ *= s;
            z_ *= s;
            return *this;
        }

        template<typename S>
        auto &operator/=(S const &s) {
            x_ /= s;
            y_ /= s;
            z_ /= s;
            return *this;
        }

    protected:
        REF_FIELDS()
    };
}

REF_IMPL_SPEC()

namespace xmd {
    template<typename E>
    auto norm(vec3_expr<E> const& e) {
        return xmd::sqrt(norm_squared(e));
    }

    template<typename E>
    auto norm_squared(vec3_expr<E> const& e) {
        return e.x() * e.x() + e.y() * e.y() + e.z() * e.z();
    }

    template<typename E>
    auto l1_norm(vec3_expr<E> const& e) {
        return xmd::abs(e.x()) + xmd::abs(e.y()) + xmd::abs(e.z());
    }

    template<typename E1, typename E2>
    auto dot(vec3_expr<E1> const& e1, vec3_expr<E2> const& e2) {
        return e1.x() * e2.x() + e1.y() * e2.y() + e1.z() * e2.z();
    }

    template<typename E>
    auto norm_inv(vec3_expr<E> const& e) {
        return decltype(norm(e))(1.0) / norm(e);
    }

    template<typename E1, typename E2>
    class sum_expr: public vec3_expr<sum_expr<E1, E2>> {
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
    auto operator+(vec3_expr<E1> const& e1, vec3_expr<E2> const& e2) {
        return sum_expr<E1, E2>(static_cast<E1 const&>(e1),
            static_cast<E2 const&>(e2));
    }

    template<typename E1, typename E2>
    class diff_expr: public vec3_expr<diff_expr<E1, E2>> {
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
    auto operator-(vec3_expr<E1> const& e1, vec3_expr<E2> const& e2) {
        return diff_expr<E1, E2>(static_cast<E1 const&>(e1),
            static_cast<E2 const&>(e2));
    }

    template<typename E>
    class neg_expr: public vec3_expr<neg_expr<E>> {
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
    auto operator-(vec3_expr<E> const& e) {
        return neg_expr<E>(static_cast<E const&>(e));
    }

    template<typename S, typename E>
    class scalar_lmul_expr: public vec3_expr<scalar_lmul_expr<S, E>> {
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
    auto operator*(S const& s, vec3_expr<E> const& e) {
        return scalar_lmul_expr<S, E>(s, static_cast<E const&>(e));
    }

    template<typename E, typename S>
    class scalar_rmul_expr: public vec3_expr<scalar_rmul_expr<E, S>> {
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
    auto operator*(vec3_expr<E> const& e, S const& s) {
        return scalar_rmul_expr<E, S>(static_cast<E const&>(e), s);
    }

    template<typename E, typename S>
    class scalar_div_expr: public vec3_expr<scalar_div_expr<E, S>> {
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
    auto operator/(vec3_expr<E> const& e, S const& s) {
        return scalar_div_expr<E, S>(static_cast<E const&>(e), s);
    }

    template<typename E1, typename E2>
    class cross_expr: public vec3_expr<cross_expr<E1, E2>> {
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
    auto cross(vec3_expr<E1> const& e1, vec3_expr<E2> const& e2) {
        return cross_expr<E1, E2>(static_cast<E1 const&>(e1),
            static_cast<E2 const&>(e2));
    }

    template<typename E>
    auto unit(vec3_expr<E> const& e) {
        return norm_inv(e) * e;
    };
}

GEN_CONST_REF()
GEN_PTR()
GEN_CONST_PTR()
GEN_SPAN()
GEN_CONST_SPAN()
GEN_MEMORY()
GEN_ALLOCATOR()
GEN_VECTOR()
GEN_SET()

namespace xmd {
    using vec3r = vec3<real>;
    using vec3tr = vec3<true_real>;
}

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace YAML {
    template<>
    struct convert<xmd::vec3r> {
        static Node encode(xmd::vec3r const& v);
        static bool decode(Node const& node, xmd::vec3r& v);
    };
}