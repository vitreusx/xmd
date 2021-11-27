#include "files/seq_file.h"
#include "map_file.h"
#include <fstream>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <regex>

namespace xmd {
    seq_file::seq_file(const std::filesystem::path &seq_file_path) {
        auto seq_file_node = YAML::LoadFile(seq_file_path);

        int res_seq_num = 1;

        for (auto const& chain_node: seq_file_node["model"]["chains"]) {
            auto res_codes = chain_node["seq"].template as<std::string>();
            res_codes = std::regex_replace(res_codes, std::regex("\\s+"), "");

            auto& xmd_chain = m.chains.emplace_back(
                std::make_unique<xmd::model::chain>());
            xmd_chain->residues.resize(res_codes.size());

            for (size_t idx = 0; idx < res_codes.size(); ++idx) {
                auto res_code = res_codes[idx];

                auto& xmd_res = m.residues.emplace_back(
                    std::make_unique<xmd::model::residue>());
                xmd_res->seq_num = res_seq_num++;
                xmd_res->parent = &*xmd_chain;
                xmd_res->type = amino_acid(res_code);
                xmd_res->pos = { 0.0, 0.0, 0.0 };

                xmd_chain->residues[idx] = &*xmd_res;
            }

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
                    model::contact m_cont;
                    m_cont.res1 = xmd_chain->residues[mf_cont.i1];
                    m_cont.res2 = xmd_chain->residues[mf_cont.i2];
                    m_cont.length = mf_cont.length;

                    m.contacts.push_back(m_cont);
                }

                for (auto const& mf_angle: mf.angles) {
                    model::angle m_angle;
                    m_angle.res1 = xmd_chain->residues[mf_angle.i1];
                    m_angle.res2 = xmd_chain->residues[mf_angle.i2];
                    m_angle.res3 = xmd_chain->residues[mf_angle.i3];
                    m_angle.theta = mf_angle.theta;

                    m.angles.push_back(m_angle);
                }

                for (auto const& mf_dihedral: mf.dihedrals) {
                    model::dihedral m_dihedral;
                    m_dihedral.res1 = xmd_chain->residues[mf_dihedral.i1];
                    m_dihedral.res2 = xmd_chain->residues[mf_dihedral.i2];
                    m_dihedral.res3 = xmd_chain->residues[mf_dihedral.i3];
                    m_dihedral.res4 = xmd_chain->residues[mf_dihedral.i4];
                    m_dihedral.phi = mf_dihedral.phi;

                    m.dihedrals.push_back(m_dihedral);
                }
            }
        }
    }

    model const &seq_file::to_model() const {
        return m;
    }
}