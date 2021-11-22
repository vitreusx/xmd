#pragma once
#include <Eigen/Core>

namespace xmd {
    class nr_device {
    public:
        explicit nr_device(int seed);

        inline double uniform();
        inline double uniform(double a, double b);

        inline double normal();
        inline double normal(double mu, double sigma);

        inline std::tuple<double, double> normal2();
        inline std::tuple<double, double> normal2(double mu, double sigma);

        Eigen::Vector3d sphere();
        Eigen::Vector3d sphere(double radius,
            Eigen::Vector3d const& center = Eigen::Vector3d::Zero());

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

#include "detail/nr_device.inl"
