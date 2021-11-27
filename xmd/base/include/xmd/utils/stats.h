#pragma once
#include <vector>
#include <functional>
#include <string>

namespace xmd {
    struct stats {
        std::string name;
        double mean, std, min_t, max_t, q25, q75;

        stats(std::string name, std::function<void()> const& fn, int nsamples);
    };

    std::ostream& operator<<(std::ostream& os, stats const& s);
}