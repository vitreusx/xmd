#pragma once
#include <chrono>
#include <xmd/types/scalar.h>

namespace xmd {
    class show_progress_bar {
    public:
        int width;

    public:
        true_real *true_t;
        real *V, *E;
        true_real total_time;
        decltype(std::chrono::high_resolution_clock::now()) start_wall_time;

    public:
        void operator()() const;
    };
}