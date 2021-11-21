#pragma once

#include <random/nr_device.h>

namespace xmd {
    nr_device::nr_device(int seed) {
        idum = -seed;
        for (auto& x: iv) x = 0;
        uniform();
    }

    double nr_device::uniform() {
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

    double nr_device::normal() {
        auto r1 = uniform(), r2 = uniform();
        auto r = sqrt(-2.0 * log(r1));
        auto theta = M_2_PI * r2;
        return r * cos(theta);
    }

    std::tuple<double, double> nr_device::normal2() {
        auto r1 = uniform(), r2 = uniform();
        auto r = sqrt(-2.0 * log(r1));
        auto theta = M_2_PI * r2;
        return { r * cos(theta), r * sin(theta) };
    }

    double nr_device::uniform(double a, double b) {
        return (b - a) * uniform() + a;
    }

    double nr_device::normal(double mu, double sigma) {
        return sigma * normal() + mu;
    }

    std::tuple<double, double> nr_device::normal2(double mu, double sigma) {
        auto [x, y] = normal2();
        return { sigma * x + mu, sigma * y + mu };
    }

    Eigen::Vector3d nr_device::sphere() {
        auto [x, y] = normal2();
        auto z = normal();
        Eigen::Vector3d p {x, y, z};
        return p.normalized();
    }

    Eigen::Vector3d nr_device::sphere(double radius,
        Eigen::Vector3d const& center) {

        auto p = sphere();
        return center + radius * p;
    }
}