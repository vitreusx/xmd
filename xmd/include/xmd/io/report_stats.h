#pragma once

#include <xmd/types/scalar.h>
#include <optional>
#include <vector>
#include <string>
#include <filesystem>
#include <xmd/stats/total_energy.h>
#include <xmd/stats/asphericity.h>
#include <xmd/stats/gyration.h>

namespace xmd {
    class report_stats {
    public:
        real period;
        std::filesystem::path csv_path;

    public:
        real *t, *V, *last_t;
        bool *first_time;
        compute_total_energy comp_tot_ene_t;
        compute_asphericity comp_asph_t;
        compute_gyration_radius comp_gyr_t;



    public:
        void operator()() const;
    };
}