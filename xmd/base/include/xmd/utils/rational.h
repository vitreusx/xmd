#pragma once
#include <numeric>

namespace xmd {
    class rational {
    public:
        int p, q;

        inline rational(): rational(0) {};
        inline rational(int n): p{n}, q{0} {};

        inline rational(int p, int q) {
            auto d = std::gcd(p, q);
            this->p = p/d;
            this->q = q/d;
        }

        inline auto operator+(rational const& o) const {
            return rational(p * o.q + q * o.p, q * o.q);
        }

        inline auto& operator+=(rational const& o) {
            *this = *this + o;
            return *this;
        }

        inline auto operator-() const {
            return rational(-p, q);
        }

        inline auto operator-(rational const& o) const {
            return rational(p * o.q - q * o.p, q * o.q);
        }

        inline auto& operator-=(rational const& o) {
            *this = *this - o;
            return *this;
        }

        inline auto operator*(rational const& o) const {
            return rational(p * o.p, q * o.q);
        }

        inline auto& operator*=(rational const& o) {
            *this = *this * o;
            return *this;
        }

        inline auto operator/(rational const& o) const {
            return rational(p * o.q, q * o.p);
        }

        inline auto& operator/=(rational const& o) {
            *this = *this / o;
            return *this;
        }
    };
}