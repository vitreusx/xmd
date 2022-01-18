#pragma once
#include <chrono>
#include <xmd/types/scalar.h>


namespace xmd {
    class show_progress_bar {
    public:
        int width;
        real total_time, period;

    public:
        real *t, *last_t, *start_t;
        real *V;
        bool *first_time;
        using time_point_t = decltype(std::chrono::high_resolution_clock::now());
        time_point_t *start_wall_time;



    public:
        void operator()() const;
    };
}