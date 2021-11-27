#include <xmd/types/vec3.h>
#include <random>
#include <functional>
#include <chrono>
#include <vector>
#include <iostream>

template<typename T>
void gen_saxpy(T x1, T x2, T x3, T y1, T y2, T y3, T z1, T z2, T z3, float a, int size) {
    for (int idx = 0; idx < size; ++idx) {
        z1[idx] = a * x1[idx] + y1[idx];
        z2[idx] = a * x2[idx] + y2[idx];
        z3[idx] = a * x3[idx] + y3[idx];
    }
}

void saxpy(float *x1, float *x2, float *x3, float *y1, float *y2, float *y3,
    float *z1, float *z2, float *z3, float a, int size) {


}

void saxpy(xmd::vec3f_array const& x, xmd::vec3f_array const& y,
    xmd::vec3f_array const& z, float a, int size) {

    for (int idx = 0; idx < size; ++idx) {
        z[idx] = a * x[idx] + y[idx];
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
        std = sqrt(var / (double)(nsamples - 1));

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

    int n = int(1) << 16;
    std::vector<float> x1(n), x2(n), x3(n);
    std::vector<float> y1(n), y2(n), y3(n);
    std::vector<float> z1(n), z2(n), z3(n);

    xmd::vec3f_array x(x1.data(), x2.data(), x3.data(), n);
    xmd::vec3f_array y(y1.data(), y2.data(), y3.data(), n);
    xmd::vec3f_array z(z1.data(), z2.data(), z3.data(), n);

    float a = dist(dev);
    for (int idx = 0; idx < n; ++idx) {
        x[idx] = xmd::vec3f(dist(dev), dist(dev), dist(dev));
        y[idx] = xmd::vec3f(dist(dev), dist(dev), dist(dev));
    }

    int nsamples = (int)1 << 16;

    std::cout << "n = " << n << "\n"
              << "nsamples = " << nsamples << "\n";

    std::cout << stats("flat, float*", [&]() -> void {
        saxpy<float*>(x1.data(), x2.data(), x3.data(),
            y1.data(), y2.data(), y3.data(),
            z1.data(), z2.data(), z3.data(),
            a, n);
    }, nsamples) << "\n";

    std::cout << stats("flat, float * __restrict__", [&]() -> void {
        saxpy<float * __restrict__>(x1.data(), x2.data(), x3.data(),
            y1.data(), y2.data(), y3.data(),
            z1.data(), z2.data(), z3.data(),
            a, n);
    }, nsamples) << "\n";

    std::cout << stats("flat, std::vector<float>", [&]() -> void {
        saxpy<std::vector<float>&>(x1, x2, x3,
            y1, y2, y3, z1, z2, z3, a, n);
    }, nsamples) << "\n";

    std::cout << stats("compact, vec3f_array ", [&]() -> void {
        saxpy(x, y, z, a, n);
    }, nsamples) << "\n";

//    auto glut_seqfile = xmd::seq_file("data/models/glut.yml");
//    auto glut_model = glut_seqfile.to_model();
//    auto rand = xmd::nr_device<Eigen::Vector3d>(442);
//    glut_model.morph_into_saw(rand, 3.8, 1e-3, true);
//    std::ofstream("glut.pdb") << xmd::pdb(glut_model);
    return 0;
}