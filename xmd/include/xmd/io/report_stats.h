#pragma once
#include <xmd/vm/vm.h>
#include <xmd/types/scalar.h>
#include <optional>
#include <vector>
#include <string>
#include <filesystem>
#include <xmd/stats/total_energy.h>

namespace xmd {
    class report_stats: public vm_aware {
    public:
        real period;
        std::filesystem::path csv_path;

    public:
        real *t, *V, *last_t;
        bool *first_time;
        compute_total_energy comp_tot_ene_t;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}