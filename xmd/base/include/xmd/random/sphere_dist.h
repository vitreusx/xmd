#pragma once
#include <random>
#include <tuple>

namespace xmd {
    template<typename VectorType, typename ScalarType>
    class sphere_dist {
    public:
        template<typename Generator>
        VectorType operator()(Generator& g) {
            auto normal = std::normal_distribution<ScalarType>();
            auto x = normal(g), y = normal(g), z = normal(g);
            return VectorType(x, y, z);
        }
    };
}