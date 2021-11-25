#include <xmd/meta/memory.h>
#include <xmd/types/vec3.h>
#include <xmd/meta/vector.h>

#include <random>
#include <iostream>
#include <chrono>

template<typename U>
std::ostream& operator<<(std::ostream& os, xmd::vec3<U> const& v) {
    return (os << "(" << v.x << ", " << v.y << ", " << v.z << ")");
}

template<typename GenT, size_t... I>
std::ostream& operator<<(std::ostream& os, xmd::gen_vector<GenT, I...> const& vs) {
    os << "[";
    for (size_t idx = 0; idx < vs.size(); ++idx) {
        os << vs[idx];
        if (idx+1 < vs.size()) os << ", ";
    }
    os << "]";
    return os;
}

//template<typename List>
//void compute_cross(List& u, List& v, List& uxv) {
//    for (size_t idx = 0; idx < u.size(); ++idx) {
//        auto _u = u[idx], _v = v[idx];
//        xmd::vec3f _uvx;
//        _uvx.
//        uxv[idx] = xmd::cross(u[idx], v[idx]);
//    }
//}

using X = float*;
void comp_cross(size_t n, X u_x, X u_y, X u_z, X v_x, X v_y, X v_z,
    X uxv_x, X uxv_y, X uxv_z) {

    for (size_t idx = 0; idx < n; ++idx) {
        uxv_x[idx] = u_y[idx] * v_z[idx] - u_z[idx] * v_y[idx];
        uxv_y[idx] = u_z[idx] * v_x[idx] - u_x[idx] * v_z[idx];
        uxv_z[idx] = u_x[idx] * v_y[idx] - u_y[idx] * v_x[idx];
    }
}

void comp_cross(std::vector<xmd::vec3f>& u, std::vector<xmd::vec3f>& v,
    std::vector<xmd::vec3f>& uxv) {

    for (size_t idx = 0; idx < u.size(); ++idx) {
        uxv[idx].x = u[idx].y * v[idx].z - u[idx].z * v[idx].y;
        uxv[idx].y = u[idx].z * v[idx].x - u[idx].x * v[idx].z;
        uxv[idx].z = u[idx].x * v[idx].y - u[idx].y * v[idx].x;
    }
}

void comp_cross(xmd::vector<xmd::vec3f>& u, xmd::vector<xmd::vec3f>& v,
    xmd::vector<xmd::vec3f>& uxv) {

    for (size_t idx = 0; idx < u.size(); ++idx) {
        uxv.x[idx] = u.y[idx] * v.z[idx] - u.z[idx] * v.y[idx];
        uxv.y[idx] = u.z[idx] * v.x[idx] - u.x[idx] * v.z[idx];
        uxv.z[idx] = u.x[idx] * v.y[idx] - u.y[idx] * v.x[idx];
//        uxv[idx].x = u[idx].y * v[idx].z - u[idx].z * v[idx].y;
//        uxv[idx].y = u[idx].z * v[idx].x - u[idx].x * v[idx].z;
//        uxv[idx].z = u[idx].x * v[idx].y - u[idx].y * v[idx].x;
    }
}

void comp_cross(size_t n, xmd::vec3f* u, xmd::vec3f* v, xmd::vec3f* uxv) {
    for (size_t idx = 0; idx < n; ++idx) {
        uxv[idx] = xmd::cross(u[idx], v[idx]);
    }
}

int main() {
    using namespace xmd;
    using namespace std::chrono;

    using List = std::vector<vec3f>;

    long seed = time(nullptr);
    std::default_random_engine eng(seed);
    std::uniform_real_distribution<float> dist;

    size_t n = size_t(1) << 18;
    List u(n), v(n), uxv(n);
    for (size_t idx = 0; idx < n; ++idx) {
        u[idx] = vec3f(dist(eng), dist(eng), dist(eng));
        v[idx] = vec3f(dist(eng), dist(eng), dist(eng));
    }

    auto before = high_resolution_clock::now();
    for (size_t repeat = 0; repeat < 1000; ++repeat) {
//        comp_cross(n, u.x.data(), u.y.data(), u.z.data(),
//            v.x.data(), v.y.data(), v.z.data(),
//            uxv.x.data(), uxv.y.data(), uxv.z.data());
        comp_cross(u, v, uxv);
    }
    auto after = high_resolution_clock::now();
    auto dur = (double)duration_cast<nanoseconds>(after - before).count() / 1e9;

    std::cout << dur << '\n';

    return 0;
}