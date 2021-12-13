#include "io/stats_reporter.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/files/csv.h>
#include <fstream>

namespace xmd {
    void stats_reporter::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        period = vm_inst.find_or_emplace<real>("stats_period",
            params["stats reporter"]["exec period"].as<quantity>());
        csv_path = vm_inst.find_or_emplace<std::filesystem::path>("stats_path",
            params["stats reporter"]["path"].as<std::string>());
        stats = vm_inst.find_or_emplace<std::vector<std::string>>("stats",
            params["stats reporter"]["stats"].as<std::vector<std::string>>());

        t = &vm_inst.find<real>("t");
        last_t = &vm_inst.find_or_emplace<real>("stats_last_t",
            std::numeric_limits<real>::lowest());
        first_time = &vm_inst.find_or_emplace<bool>("stats_first_time", true);

        for (auto const& stat: stats) {
            if (stat == "V") {
                V = &vm_inst.find<real>("V");
            }
            else if (stat == "K" || stat == "E") {
                comp_tot_ene_t.init_from_vm(vm_inst);
            }
        }
    }

    void stats_reporter::operator()() const {
        if (*t - *last_t >= period) {
            csv_file stats_csv;
            stats_csv.set_header(stats);

            auto& record = stats_csv.add_record();

            bool computed_KE = false;
            for (auto const& stat: stats) {
                if (stat == "t") {
                    record["t"] = std::to_string(*t);
                }
                else if (stat == "V") {
                    record["V"] = std::to_string(*V);
                }
                else if (stat == "K" || stat == "E") {
                    if (!computed_KE) {
                        comp_tot_ene_t();
                        computed_KE = true;
                    }

                    if (stat == "K") {
                        record["K"] = std::to_string(*comp_tot_ene_t.K);
                    }
                    else if (stat == "E") {
                        record["E"] = std::to_string(*comp_tot_ene_t.E);
                    }
                }
            }

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