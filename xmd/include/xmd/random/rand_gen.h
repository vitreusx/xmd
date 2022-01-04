#pragma once
#include <tuple>
#include <xmd/utils/math.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class rand_gen {
    public:
        uint64_t state;

        inline explicit rand_gen(uint64_t seed) {
            state = seed;
        }

        inline rand_gen spawn() {
            uint64_t seed = ((*this)() + 0x9E3779B97f4A7C15);
            seed = (seed ^ (seed >> 30)) * 0xBF58476D1CE4E5B9;
            seed = (seed ^ (seed >> 27)) * 0x94D049BB133111EB;
            return rand_gen(seed ^ (seed >> 31));
        }

        inline uint64_t operator()() {
            auto res = state * 0xd989bcacc137dcd5ull;
            state ^= state >> 11;
            state ^= state << 31;
            state ^= state >> 18;
            return res;
        }

        template<typename U>
        inline U uniform() {
            auto inv = (U)1.0 / (U)(1ull << 32);
            auto val = (*this)();
            return (U)(val >> 32ull) * inv;
        }

        template<typename U>
        inline U uniform(U a, U b) {
            return (b - a) * uniform<U>() + a;
        }

        template<typename U>
        inline U normal() {
            auto r1 = uniform<U>(), r2 = uniform<U>();
            U r = sqrt((U)(-2.0) * log(r1));
            if (isnan(r)) r = (U)0.0;
            U t = (U)(2.0 * M_PI) * r2;
            return r * cos(t);
        }

        template<typename U>
        inline std::pair<U, U> normal_x2() {
            auto r1 = uniform<U>(), r2 = uniform<U>();
            U r = sqrt((U)(-2.0) * log(r1));
            if (isnan(r)) r = (U)0.0;
            U t = (U)(2.0 * M_PI) * r2;
            return std::make_pair(r * cos(t), r * sin(t));
        }

        template<typename U>
        inline vec3<U> sphere() {
            auto [x, y] = normal_x2<U>();
            auto z = normal<U>();
            return unit(vec3<U>(x, y, z));
        }
    };
}