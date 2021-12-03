#pragma once

namespace xmd {
    class splitmix64 {
    public:
        uint64_t state;
        inline explicit splitmix64(uint64_t seed):
            state {seed} {};

        inline uint64_t operator()() {
            uint64_t result = (state += 0x9E3779B97f4A7C15);
            result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
            result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
            return result ^ (result >> 31);
        }
    };
}