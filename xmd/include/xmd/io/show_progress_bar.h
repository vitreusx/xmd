#pragma once
#include <chrono>
#include <xmd/types/scalar.h>
#include <xmd/ctx/context.h>

namespace xmd {
    class show_progress_bar: public ctx_aware {
    public:
        int width;
        real total_time, period;

    public:
        real *t, *last_t, *start_t;
        real *V;
        bool *first_time;
        using time_point_t = decltype(std::chrono::high_resolution_clock::now());
        time_point_t *start_wall_time;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}