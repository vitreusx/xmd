#include "forces/primitives/lj_variants.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/types/amino_acid.h>
#include <xmd/files/csv.h>

namespace xmd {
    lj_variants::lj_variants(const yaml_fs_node &lj_params) {
        bb.r_min() = lj_params["bb"]["r_min"].as<quantity>();
        bb.depth() = lj_params["bb"]["depth"].as<quantity>();

        bs.r_min() = lj_params["bs"]["r_min"].as<quantity>();
        bs.depth() = lj_params["bs"]["depth"].as<quantity>();

        sb = bs;

        auto num_ss_lj_types = amino_acid::NUM_AA * amino_acid::NUM_AA;

        auto mut_ss =  ctx.persistent<vector<sink_lj>>("ss_ljs_vector",
            num_ss_lj_types).view();
        ss = mut_ss;

        if (auto def_r_max_p = lj_params["ss"]["default"]["r_max"]; def_r_max_p) {
            auto def_r_max = def_r_max_p.as<quantity>();
            for (int ss_idx = 0; ss_idx < num_ss_lj_types; ++ss_idx)
                mut_ss[ss_idx].r_max() = def_r_max;
        }

        if (auto def_depth_p = lj_params["ss"]["default"]["depth"]; def_depth_p) {
            auto def_depth = def_depth_p.as<quantity>();
            for (int ss_idx = 0; ss_idx < num_ss_lj_types; ++ss_idx)
                mut_ss[ss_idx].depth() = def_depth;
        }

        if (auto per_pair_ss = lj_params["ss"]["per pair"]; per_pair_ss) {
            if (auto r_max_p = per_pair_ss["r_max"]; r_max_p) {
                auto r_min_csv = r_max_p.as<csv_file>();
                for (auto const& rec: r_min_csv.records) {
                    auto aa1 = amino_acid(rec["type"]);
                    for (auto const& aa2: amino_acid::all()) {
                        int ss_idx = amino_acid::NUM_AA*(int)aa1 + (int)aa2;
                        mut_ss[ss_idx].r_max() = quantity(rec[aa2.name()], angstrom);
                    }
                }
            }

            if (auto depth_p = per_pair_ss["depth"]; depth_p) {
                auto depth_csv = depth_p.as<csv_file>();
                for (auto const& rec: depth_csv.records) {
                    auto aa1 = amino_acid(rec["type"]);
                    for (auto const& aa2: amino_acid::all()) {
                        int ss_idx = amino_acid::NUM_AA*(int)aa1 + (int)aa2;
                        mut_ss[ss_idx].depth() = quantity(rec[aa2.name()], eps);
                    }
                }
            }
        }

        bool sinking = lj_params["ss"]["use sinking variant"].as<bool>();
        for (int ss_idx = 0; ss_idx < num_ss_lj_types; ++ss_idx) {
            mut_ss[ss_idx].r_min() = sinking ? bb.r_min() : mut_ss[ss_idx].r_max();
        }
    }
}