#pragma once
#include <xmd/random/splitmix64.h>
#include <tuple>
#include <xmd/math.h>

namespace xmd {
    class xorshift64 {
    public:
        uint64_t state;

        inline explicit xorshift64(uint64_t seed) {
            state = seed;
        }

        inline uint64_t operator()() {
            auto res = state;
            res ^= res << 13;
            res ^= res >> 7;
            res ^= res << 17;
            return (state = res);
        }

        inline std::pair<float, float> normalx2() {
            uint64_t val1 = (*this)();
            float r1 = (float)((uint32_t)val1) / (float)(1ull << 32) + 1.0e-8f;
            float r = sqrt(-2.0f * log(r1));
            float r2 = (float)((uint32_t)(val1 >> 32)) / (float)(1ull << 32);
            float t = 2.0f * (float)M_PI * r2;

            return { r * cos(t), r * sin(t) };
        }
    };
}