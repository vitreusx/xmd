#include "freeform/main.h"
#include "freeform/params.h"
#include "defaults.h"
#include <xmd/utils/yaml.h>
#include <string>
#include <iostream>
#include <xmd/model/model.h>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <xmd/random/rand_gen.h>
#include <fstream>
#include <unordered_map>

namespace xmd {
    int freeform_main::operator()(int argc, char **argv) {
        std::vector<std::string> argv_s(argc);
        for (int idx = 0; idx < argc; ++idx)
            argv_s[idx] = argv[idx];

        auto print_help = [&]() -> void {
            std::cout << "Usage: " << argv_s[0] << " [--help|file]" << '\n';
            std::cout << '\t' << "file" << '\t' << "YAML parameter file" << '\n';
            std::cout << '\t' << "--help" << '\t' << "display this help message";
        };

        if (argc == 2 && argv_s[1] == "--help") {
            print_help();
            return EXIT_SUCCESS;
        }
        else if (argc > 2) {
            print_help();
            return EXIT_FAILURE;
        }

        yaml_fs_node params_fs_node;
        if (argc == 2) {
            auto param_path = argv_s[1];
            auto overrides = YAML::LoadFile(param_path);
            auto params_node = merge_yaml(defaults_yml(), overrides);
            params_fs_node = yaml_fs_node(params_node, param_path);
        }
        else {
            params_fs_node = yaml_fs_node(defaults_yml(), ".");
        }

        freeform_params params(params_fs_node);

        auto& gen = st.make_temp<xmd::rand_gen>("gen", params.general.seed);

        xmd::model model;
        auto& model_file_v = params.model.file;
        if (std::holds_alternative<model_params::seqfile_params>(model_file_v)) {
            auto& seqfile_p = std::get<model_params::seqfile_params>(model_file_v);
            auto seqfile  = seq_file(seqfile_p.path);
            model = seqfile.to_model();
        }
        else if (std::holds_alternative<model_params::pdbfile_params>(model_file_v)) {
            auto& pdbfile_p = std::get<model_params::pdbfile_params>(model_file_v);
            auto stream = std::ifstream(pdbfile_p.path);
            auto pdbfile = pdb(std::move(stream));
            model = pdbfile.to_model();
        }

        if (params.model.morph_into_saw.has_value()) {
            auto& saw_p = params.model.morph_into_saw.value();
            model.morph_into_saw(gen, saw_p.bond_distance, saw_p.residue_density,
                saw_p.infer_box);
        }

        using res_map_t  = std::unordered_map<xmd::model::residue*, int>;
        res_map_t res_map;

        int res_idx = 0;
        for (auto const& res: model.residues)
            res_map[res.get()] = res_idx++;

        auto& num_res = st.make_temp<int>("num_res", model.residues.size());

        auto& r = st.make_saved<vector<vec3r>>("r", [&]() -> auto {
            vector<vec3r> r_(num_res);
            for (auto const& res: model.residues)
                r_[res_map.at(res.get())] = res->pos;
            return r_;
        });

        auto& atype = st.make_saved<vector<amino_acid>>("atype", [&]() -> auto {
            vector<amino_acid> atype_(num_res);
            for (auto const& res: model.residues)
                atype_[res_map.at(res.get())] = res->type;
        });

        auto& mass = st.make_temp<vector<real>>("mass", num_res, (real)1.0);
        auto& box = st.make_saved<xmd::box>("box", [&]() -> auto {
            return model.model_box;
        });

        auto& prev = st.make_saved<vector<int>>("prev", [&]() -> auto {
            vector<int> prev_(num_res, -1);
            for (auto const& res: model.residues) {
                if (res->seq_idx > 0) {
                    auto prev_ptr = res->parent->residues[res->seq_idx-1];
                    prev_[res_map.at(res.get())] = res_map.at(prev_ptr);
                }
            }
            return prev_;
        });

        auto& next = st.make_saved<vector<int>>("next", [&]() -> auto {
            vector<int> next_(num_res, -1);
            for (auto const& res: model.residues) {
                int num_res_in_chain = res->parent->residues.size();
                if (res->seq_idx < num_res_in_chain-1) {
                    auto next_ptr = res->parent->residues[res->seq_idx+1];
                    next_[res_map.at(res.get())] = res_map.at(next_ptr);
                }
            }
            return next_;
        });

        auto& chain_idx = st.make_saved<vector<int>>("chain_idx", [&]() -> auto {
            vector<int> chain_idx_(num_res, -1);
            for (auto const& res: model.residues) {
                chain_idx_[res_map.at(res.get())] = res->parent->chain_idx;
            }
            return chain_idx_;
        });

        auto& seq_idx = st.make_saved<vector<int>>("seq_idx", [&]() -> auto {
            vector<int> seq_idx_(num_res, -1);
            for (auto const& res: model.residues) {
                seq_idx_[res_map.at(res.get())] = res->seq_idx;
            }
            return seq_idx_;
        });



        return EXIT_SUCCESS;
    }
}