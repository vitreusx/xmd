#pragma once
#include <chrono>

namespace xmd {
    class show_progress_bar {
    public:
        int width;

    public:
        double *true_t;
        float *V;
        double total_time;
        decltype(std::chrono::high_resolution_clock::now()) start_wall_time;

    public:
        void operator()() const;
    };
}