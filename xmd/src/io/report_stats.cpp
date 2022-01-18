#include "io/report_stats.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/files/csv.h>
#include <fstream>

namespace xmd {
    void report_stats::operator()() const {
        if (*t - *last_t >= period) {
            csv_file stats_csv;
            stats_csv.set_header({ "t[tau]", "V[eps]", "K[eps]", "E[eps]", "Rg[A]", "W[A]" });

            auto& record = stats_csv.add_record();

            comp_tot_ene_t();
            comp_asph_t();
            comp_gyr_t();

            record["t[tau]"] = std::to_string(*t / tau);
            record["V[eps]"] = std::to_string(*V / eps);
            record["K[eps]"] = std::to_string(*comp_tot_ene_t.K / eps);
            record["E[eps]"] = std::to_string(*comp_tot_ene_t.E / eps);
            record["Rg[A]"] = std::to_string(*comp_gyr_t.gyration_radius / angstrom);
            record["W[A]"] = std::to_string(*comp_asph_t.asphericity / angstrom);

            if (*first_time) {
                std::ofstream csv_file_stream(csv_path);
                stats_csv.print(csv_file_stream, true);
                csv_file_stream.flush();
                *first_time = false;
            }
            else {
                std::ofstream csv_file_stream(csv_path, std::ios::app);
                stats_csv.print(csv_file_stream, false);
                csv_file_stream.flush();
            }

            *last_t = *t;
            *first_time = false;
        }
    }
}