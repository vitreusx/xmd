#include "utils/stats.h"
#include <xmd/math.h>
#include <chrono>
#include <numeric>

namespace xmd {
    stats::stats(std::string name, const std::function<void()> &fn,
        int nsamples)  {

        std::vector<true_real> times(nsamples);

        using namespace std::chrono;
        for (int sample_idx = 0; sample_idx < nsamples; ++sample_idx) {
            auto before = high_resolution_clock::now();
            fn();
            auto after = high_resolution_clock::now();
            auto dur_ns = duration_cast<nanoseconds>(after - before).count();
            times[sample_idx] = (true_real)dur_ns / 1.0e9;
        }

        mean = std::accumulate(times.begin(), times.end(), 0.0) / (true_real)nsamples;
        true_real var = std::accumulate(times.begin(), times.end(), 0.0,
            [this](auto cur_var, auto t) -> auto {
                return cur_var + (t - mean) * (t - mean);
            });
        std = nsamples > 1 ? sqrt(var / (true_real)(nsamples - 1)) : 0.0;

        std::sort(times.begin(), times.end());
        min_t = times.front();
        max_t = times.back();
        q25 = times[(int)(0.25 * (true_real)nsamples)];
        q75 = times[(int)(0.75 * (true_real)nsamples)];

        this->name = std::move(name);
    }

    std::ostream& operator<<(std::ostream& os, stats const& s)  {
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
}