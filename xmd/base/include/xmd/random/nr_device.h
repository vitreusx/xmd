#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    template<typename VectorT>
    class nr_device {
    public:
        inline explicit nr_device(int seed) {
            idum = -seed;
            for (auto& x: iv) x = 0;
            uniform();
        }

        inline double uniform() {
            int k, j;
            if (idum <= 0) {
                idum2 = idum = std::max(-idum, 1);
                for (j = ntab + 7; j >= 0; --j) {
                    k = idum / iq1;
                    idum = ia1 * (idum - k * iq1) - k * ir1;
                    if (idum < 0) idum += im1;
                    if (j < ntab) iv[j] = idum;
                }
                iy = iv[0];
            }

            k = idum / iq1;
            idum = ia1 * (idum - k * iq1) - k * ir1;
            if (idum < 0) idum += im1;

            k = idum2 / iq2;
            idum2 = ia2 * (idum2 - k * iq2) - k * ir2;
            if (idum2 < 0) idum2 += im2;

            j = iy / ndiv;
            iy = iv[j] - idum2;
            iv[j] = idum;
            if (iy < 1) iy += imm1;

            return std::min(am * iy, rnmx);
        }

        inline double uniform(double a, double b) {
            return (b - a) * uniform() + a;
        }

        inline double normal() {
            auto r1 = uniform(), r2 = uniform();
            auto r = sqrt(-2.0 * log(r1));
            auto theta = M_2_PI * r2;
            return r * cos(theta);
        }

        inline double normal(double mu, double sigma) {
            return sigma * normal() + mu;
        }

        inline std::tuple<double, double> normal2() {
            auto r1 = uniform(), r2 = uniform();
            auto r = sqrt(-2.0 * log(r1));
            auto theta = M_2_PI * r2;
            return { r * cos(theta), r * sin(theta) };
        }

        inline std::tuple<double, double> normal2(double mu, double sigma) {
            auto [x, y] = normal2();
            return { sigma * x + mu, sigma * y + mu };
        }

        VectorT sphere() {
            auto [x, y] = normal2();
            auto z = normal();
            return xmd::unit(VectorT(x, y, z));
        }

        VectorT sphere(double radius, VectorT const& center = VectorT::Zero()) {
            return center + radius * sphere();
        }

    private:
        static constexpr int
            im1 = 2147483563,
            im2 = 2147483399,
            imm1 = im1-1,
            ia1 = 40014,
            ia2 = 40692,
            iq1 = 53668,
            iq2 = 52774,
            ir1 = 12211,
            ir2 = 3791,
            ntab = 32,
            ndiv = 1+imm1/ntab;

        static constexpr double
            eps = 1.2e-7,
            rnmx = 1.0-eps,
            am = 1.0/im1;

        int iy = 0, idum = -448, idum2 = 123456789;
        int iv[ntab];
    };
}
