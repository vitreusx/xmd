#pragma once
#include <xmd/vm/vm.h>
#include <xmd/types/scalar.h>
#include <variant>
#include <vector>
#include <string>
#include <filesystem>
#include <xmd/stats/total_energy.h>

namespace xmd {
    class stats_reporter: public vm_aware {
    public:
        real period;
        std::vector<std::string> stats;
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