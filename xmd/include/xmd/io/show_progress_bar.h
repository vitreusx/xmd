#pragma once
#include <chrono>
#include <xmd/types/scalar.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class show_progress_bar: public vm_aware {
    public:
        int width;
        true_real total_time;
        decltype(std::chrono::high_resolution_clock::now()) start_wall_time;

    public:
        true_real *true_t;
        real *V, *E;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}