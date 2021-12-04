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

        template<typename RealT>
        inline RealT normal() {
            uint64_t val1 = (*this)();
            RealT r1 = (RealT)((uint32_t)val1) / (RealT)(1ull << 32);
            RealT r = sqrt((RealT)(-2.0) * log(r1));
            if (isnan(r)) r = (RealT)0.0;
            RealT r2 = (RealT)((uint32_t)(val1 >> 32)) / (RealT)(1ull << 32);
            RealT t = (RealT)(2.0 * M_PI) * r2;
            return r * cos(t);
        }

        template<typename RealT>
        inline std::pair<RealT, RealT> normalx2() {
            uint64_t val1 = (*this)();
            RealT r1 = (RealT)((uint32_t)val1) / (RealT)(1ull << 32);
            RealT r = sqrt((RealT)(-2.0) * log(r1));
            if (isnan(r)) r = (RealT)0.0;
            RealT r2 = (RealT)((uint32_t)(val1 >> 32)) / (RealT)(1ull << 32);
            RealT t = (RealT)(2.0 * M_PI) * r2;
            return { r * cos(t), r * sin(t) };
        }
    };
}