#include "files/seq_file.h"
#include "map_file.h"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <regex>
#include <set>

namespace xmd {
    seq_file::seq_file(const std::filesystem::path &seq_file_path) {
        auto seq_file_node = YAML::LoadFile(seq_file_path);

        int res_seq_idx = 0;

        for (auto const& chain_node: seq_file_node["model"]["chains"]) {
            auto res_codes = chain_node["seq"].template as<std::string>();
            res_codes = std::regex_replace(res_codes, std::regex("\\s+"), "");

            auto& xmd_chain = xmd_model.chains.emplace_back(
                std::make_unique<xmd::model::chain>());
            xmd_chain->residues.resize(res_codes.size());

            for (size_t idx = 0; idx < res_codes.size(); ++idx) {
                auto res_code = res_codes[idx];

                auto& xmd_res = xmd_model.residues.emplace_back(
                    std::make_unique<xmd::model::residue>());
                xmd_res->seq_idx = res_seq_idx++;
                xmd_res->parent = &*xmd_chain;
                xmd_res->type = amino_acid(res_code);
                xmd_res->pos = { 0.0, 0.0, 0.0 };

                xmd_chain->residues[idx] = &*xmd_res;
            }

            std::set<std::tuple<model::residue*, model::residue*,
                model::residue*>> taken_angles;
            std::set<std::tuple<model::residue*, model::residue*,
                model::residue*, model::residue*>> taken_dihedrals;

            auto maps_node = chain_node["maps"];
            for (YAML::const_iterator iter = maps_node.begin(); iter != maps_node.end(); ++iter) {
                auto map_entry_node = iter->second;

                YAML::Node map_node;
                if (auto from_file_node = map_entry_node["from-file"]; from_file_node) {
                    auto rel_path = from_file_node.template as<std::string>();
                    auto map_path = seq_file_path.parent_path() / rel_path;
                    map_node = YAML::LoadFile(map_path);
                }
                else if (auto _map_node = map_entry_node["map"]; _map_node) {
                    map_node = _map_node;
                }

                auto mf = map_node.as<map_file>();
                if (auto shift_node = map_entry_node["shift"]; shift_node) {
                    auto shift_val = shift_node.template as<int>();
                    mf.shift(shift_val);
                }

                for (auto const& mf_cont: mf.contacts) {
                    auto& xmd_cont = xmd_model.contacts.emplace_back();
                    xmd_cont.res1 = xmd_chain->residues[mf_cont.i1];
                    xmd_cont.res2 = xmd_chain->residues[mf_cont.i2];
                    xmd_cont.length = mf_cont.length;
                    xmd_cont.type = model::UNKNOWN;
                }

                for (auto const& mf_angle: mf.angles) {
                    auto& xmd_angle = xmd_model.angles.emplace_back();
                    xmd_angle.res1 = xmd_chain->residues[mf_angle.i1];
                    xmd_angle.res2 = xmd_chain->residues[mf_angle.i2];
                    xmd_angle.res3 = xmd_chain->residues[mf_angle.i3];
                    xmd_angle.theta = mf_angle.theta;

                    taken_angles.insert(std::make_tuple(
                        xmd_angle.res1, xmd_angle.res2, xmd_angle.res3));
                }

                for (auto const& mf_dihedral: mf.dihedrals) {
                    auto& xmd_dihedral = xmd_model.dihedrals.emplace_back();
                    xmd_dihedral.res1 = xmd_chain->residues[mf_dihedral.i1];
                    xmd_dihedral.res2 = xmd_chain->residues[mf_dihedral.i2];
                    xmd_dihedral.res3 = xmd_chain->residues[mf_dihedral.i3];
                    xmd_dihedral.res4 = xmd_chain->residues[mf_dihedral.i4];
                    xmd_dihedral.phi = mf_dihedral.phi;

                    taken_dihedrals.insert(std::make_tuple(
                        xmd_dihedral.res1, xmd_dihedral.res2,
                        xmd_dihedral.res3, xmd_dihedral.res4));
                }
            }

            for (size_t idx = 0; idx+1 < xmd_chain->residues.size(); ++idx) {
                auto* res1 = xmd_chain->residues[idx];
                auto* res2 = xmd_chain->residues[idx+1];

                auto& xmd_tether = xmd_model.tethers.emplace_back();
                xmd_tether.res1 = res1;
                xmd_tether.res2 = res2;
                xmd_tether.length = std::nullopt;

                if (idx+2 < xmd_chain->residues.size()) {
                    auto* res3 = xmd_chain->residues[idx+2];

                    bool taken_angle = taken_angles.count(std::make_tuple(
                        res1, res2, res3)) > 0;

                    if (!taken_angle) {
                        auto& xmd_angle = xmd_model.angles.emplace_back();
                        xmd_angle.res1 = res1;
                        xmd_angle.res2 = res2;
                        xmd_angle.res3 = res3;
                        xmd_angle.theta = std::nullopt;
                    }

                    if (idx+3 < xmd_chain->residues.size()) {
                        auto* res4 = xmd_chain->residues[idx+3];

                        bool taken_dihedral = taken_dihedrals.count(std::make_tuple(
                            res1, res2, res3, res4)) > 0;

                        if (!taken_dihedral) {
                            auto& xmd_dihedral = xmd_model.dihedrals.emplace_back();
                            xmd_dihedral.res1 = res1;
                            xmd_dihedral.res2 = res2;
                            xmd_dihedral.res3 = res3;
                            xmd_dihedral.res4 = res4;
                            xmd_dihedral.phi = std::nullopt;
                        }
                    }
                }
            }
        }
    }

    model const &seq_file::to_model() const {
        return xmd_model;
    }
}