#include <xmd/types/vec3.h>
#include <random>
#include <functional>
#include <chrono>
#include <vector>
#include <iostream>
#include <Eigen/Core>

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
            x_ = e.x();
            y_ = e.y();
            z_ = e.z();
            return *this;
        }

        template<typename E>
        auto& operator+=(expr<E> const& e) {
            x_ += e.x();
            y_ += e.y();
            z_ += e.z();
            return *this;
        }

        template<typename E>
        auto& operator-=(expr<E> const& e) {
            x_ -= e.x();
            y_ -= e.y();
            z_ -= e.z();
            return *this;
        }

        template<typename S>
        auto& operator*=(S const& s) {
            x_ *= s;
            y_ *= s;
            z_ *= s;
            return *this;
        }

        template<typename S>
        auto& operator/=(S const& s) {
            x_ /= s;
            y_ /= s;
            z_ /= s;
            return *this;
        }

    public:
        U const& x() const {
            return x_;
        }

        U const& y() const {
            return y_;
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

    template<typename E>
    std::ostream& operator<<(std::ostream& os, expr<E> const& e) {
        os << "(" << e.x() << ", " << e.y() << ", " << e.z() << ")";
        return os;
    }

    using vectorf = vector<float>;
    using vectorf_array = vector_array<float>;
}

//class vec3f_array_r {
//public:
//    vec3f_array_r() = default;
//    vec3f_array_r(float *__restrict__ x, float *__restrict__ y, float *__restrict__ z, int size):
//        x{x}, y{y}, z{z}, size{size} {};
//
//    xmd::vec3<float& __restrict__> operator[](int idx) const {
//        return { x[idx], y[idx], z[idx] };
//    }
//
//    float *__restrict__ x = nullptr;
//    float *__restrict__ y = nullptr;
//    float *__restrict__ z = nullptr;
//    int size = 0;
//};

//void saxpy_xmd_r(vec3f_array_r const& __restrict__ x, vec3f_array_r const& y,
//    vec3f_array_r const& z, float a, int size) {
//
//    for (int idx = 0; idx < size; ++idx) {
//        z[idx] = a * x[idx] + y[idx];
//    }
//}
//
//void saxpy_ec(Eigen::ArrayX3f const& x, Eigen::ArrayX3f const& y, Eigen::ArrayX3f& z,
//    float a, int size) {
//
//    for (int idx = 0; idx < size; ++idx) {
//        z.row(idx) = a * x.row(idx) + y.row(idx);
//    }
//}
//
//void saxpy_v3(v3::vectorf_array const& x, v3::vectorf_array const& y,
//    v3::vectorf_array& z, float a, int size) {
//
//    for (int idx = 0; idx < size; ++idx) {
//        z[idx] = a * x[idx] + y[idx];
//    }
//}

void complex_xmd(xmd::vec3f_array const& x, xmd::vec3f_array const& y,
    xmd::vec3f_array& z, int size) {

    for (int idx = 0; idx < size; ++idx) {
        z[idx] = xmd::cross(x[idx], y[idx]) * xmd::norm_inv(x[idx] + y[idx]);
    }
}

void complex_v3(v3::vectorf_array const& x, v3::vectorf_array const& y,
    v3::vectorf_array& z, int size) {

    for (int idx = 0; idx < size; ++idx) {
        z[idx] = v3::cross(x[idx], y[idx]) * v3::norm_inv(x[idx] + y[idx]);
    }
}

struct stats {
    std::string name;
    double mean, std, min_t, max_t, q25, q75;

    stats(std::string name, std::function<void()> const& fn, int nsamples) {
        std::vector<double> times(nsamples);

        using namespace std::chrono;
        for (int sample_idx = 0; sample_idx < nsamples; ++sample_idx) {
            auto before = high_resolution_clock::now();
            fn();
            auto after = high_resolution_clock::now();
            auto dur_ns = duration_cast<nanoseconds>(after - before).count();
            times[sample_idx] = (double)dur_ns / 1.0e9;
        }

        mean = std::accumulate(times.begin(), times.end(), 0.0) / (double)nsamples;
        double var = std::accumulate(times.begin(), times.end(), 0.0,
            [this](auto cur_var, auto t) -> auto {
                return cur_var + (t - mean) * (t - mean);
            });
        std = nsamples > 1 ? sqrt(var / (double)(nsamples - 1)) : 0.0;

        std::sort(times.begin(), times.end());
        min_t = times.front();
        max_t = times.back();
        q25 = times[(int)(0.25 * (double)nsamples)];
        q75 = times[(int)(0.75 * (double)nsamples)];

        this->name = std::move(name);
    }
};

std::ostream& operator<<(std::ostream& os, stats const& s) {
    auto saved = os.flags();
    os << std::scientific;
    os << s.name << "\n"
        << "\t" << "mean = " << s.mean << "\n"
        << "\t" << "std = " << s.std << "\n"
        << "\t" << "min = " << s.min_t << "\n"
        << "\t" << "25th quantile = " << s.q25 << "\n"
        << "\t" << "75th quantile = " << s.q75 << "\n"
        << "\t" << "max = " << s.max_t;
    os.flags(saved);
    return os;
}

int main() {
    std::random_device dev;
    std::uniform_real_distribution<float> dist;

    int n = 1 << 16;
    int nsamples = 1 << 17;

    std::cout << "n = " << n << "\n"
              << "nsamples = " << nsamples << "\n";

    float a = dist(dev);

    Eigen::ArrayX3f x = Eigen::ArrayX3f::Random(n, 3),
        y = Eigen::ArrayX3f::Random(n, 3),
        z = Eigen::ArrayX3f::Random(n, 3),
        true_z = a * x + y;

    float *x1 = x.col(0).data(), *x2 = x.col(1).data(), *x3 = x.col(2).data();
    float *y1 = y.col(0).data(), *y2 = y.col(1).data(), *y3 = y.col(2).data();
    float *z1 = z.col(0).data(), *z2 = z.col(1).data(), *z3 = z.col(2).data();

    v3::vectorf_array v3_x(x1, x2, x3, n), v3_y(y1, y2, y3, n), v3_z(z1, z2, z3, n);

    xmd::vec3f_array xmd_x(x1, x2, x3, n);
    xmd::vec3f_array xmd_y(y1, y2, y3, n);
    xmd::vec3f_array xmd_z(z1, z2, z3, n);

    auto perform_ = [&](std::string const& name, auto fn) -> void {
        z.setZero();
        std::cout << stats(name, fn, nsamples) << "\n";
    };

    perform_("v3#1", [&]() -> void {
        complex_v3(v3_x, v3_y, v3_z, n);
    });

    perform_("xmd#1", [&]() -> void {
        complex_xmd(xmd_x, xmd_y, xmd_z, n);
    });

    perform_("v3#2", [&]() -> void {
        complex_v3(v3_x, v3_y, v3_z, n);
    });

    perform_("xmd#2", [&]() -> void {
        complex_xmd(xmd_x, xmd_y, xmd_z, n);
    });

    return 0;
}