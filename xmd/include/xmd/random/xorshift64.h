#pragma once
#include <xmd/random/splitmix64.h>
#include <tuple>
#include <xmd/utils/math.h>

namespace xmd {
    class xorshift64 {
    public:
        uint64_t state;

        inline explicit xorshift64(uint64_t seed) {
            state = seed;
        }

        inline uint64_t operator()() {
            auto res = state * 0xd989bcacc137dcd5ull;
            state ^= state >> 11;
            state ^= state << 31;
            state ^= state >> 18;
            return res;
        }

        template<typename RealT>
        inline RealT normal() {
            auto inv = (RealT)1.0 / (RealT) (1ull << 32);
            uint64_t val1 = (*this)(), val2 = (*this)();
            RealT r1 = (RealT)(val1 >> 32ull) * inv;
            RealT r = sqrt((RealT)(-2.0) * log(r1));
//            if (isnan(r)) r = (RealT)0.0;
            RealT r2 = (RealT)(val2 >> 32ull) * inv;
            RealT t = (RealT)(2.0 * M_PI) * r2;
            return r * cos(t);
        }

        template<typename RealT>
        inline std::pair<RealT, RealT> normalx2() {
            auto inv = (RealT)1.0 / (RealT) (1ull << 32);
            uint64_t val1 = (*this)(), val2 = (*this)();
            RealT r1 = (RealT)(val1 >> 32ull) * inv;
            RealT r = sqrt((RealT)(-2.0) * log(r1));
//            if (isnan(r)) r = (RealT)0.0;
            RealT r2 = (RealT)(val2 >> 32ull) * inv;
            RealT t = (RealT)(2.0 * M_PI) * r2;
            return { r * cos(t), r * sin(t) };
        }
    };
}