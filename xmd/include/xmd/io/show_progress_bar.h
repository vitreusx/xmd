#pragma once
#include <chrono>
#include <xmd/types/scalar.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class show_progress_bar: public vm_aware {
    public:
        int width;
        real total_time, period;

    public:
        real *t, *last_t, *start_t;
        real *V;
        bool *first_time;
        using time_point_t = decltype(std::chrono::high_resolution_clock::now());
        time_point_t *start_wall_time;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}