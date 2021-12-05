#pragma once
#include <vector>
#include <functional>
#include <string>
#include <types/scalar.h>

namespace xmd {
    struct stats {
        std::string name;
        true_real mean, std, min_t, max_t, q25, q75;

        stats(std::string name, std::function<void()> const& fn, int nsamples);
    };

    std::ostream& operator<<(std::ostream& os, stats const& s);
}