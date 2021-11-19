#include "param_file.h"
#include <yaml-cpp/yaml.h>
#include "detail/csv.h"
#include <fstream>

namespace xmd {
    void params::specific_lj::load_from(const YAML::Node &node,
        std::filesystem::path const& pwd) {

        if (auto default_node = node["default"]; default_node) {
            if (auto sigma_node = default_node["sigma"]; sigma_node) {
                def_sigma = sigma_node.as<double>();
            }
            if (auto depth_node = default_node["depth"]; depth_node) {
                def_depth = depth_node.as<double>();
            }
        }

        auto from_csv = [&](YAML::Node const& csv_node, value_for_pair& v) -> auto {
            xmd::csv_file csv;
            if (auto from_file_node = csv_node["from-file"]; from_file_node) {
                auto path = pwd / from_file_node.as<std::string>();
                csv = csv_file(std::ifstream(path));
            }
            else {
                csv = csv_file(csv_node.as<std::string>());
            }

            auto const& header = csv.header.value();
            for (auto const& record: csv.records) {
                amino_acid aa1(record[0]);
                for (size_t i = 1; i < record.fields.size(); ++i) {
                    amino_acid aa2(header[i]);
                    v[aa1][aa2] = std::stod(record[i]);
                }
            }
        };

        if (auto pairwise_node = node["pairwise"]; pairwise_node) {
            if (auto sigma_node = pairwise_node["sigma"]; sigma_node) {
                pairwise_sigma = value_for_pair();
                from_csv(sigma_node, pairwise_sigma.value());
            }
            if (auto depth_node = pairwise_node["depth"]; depth_node) {
                pairwise_depth = value_for_pair();
                from_csv(depth_node, pairwise_depth.value());
            }
        }
    }

    void params::lj::load_from(const YAML::Node &node, std::filesystem::path const& pwd) {
        if (auto bb_node = node["bb"]; bb_node) {
            bb.load_from(bb_node, pwd);
        }
        if (auto bs_node = node["bs"]; bs_node) {
            bs.load_from(bs_node, pwd);
        }
        if (auto ss_node = node["ss"]; ss_node) {
            ss.load_from(ss_node, pwd);
        }
    }

    void params::amino_acids::load_from(const YAML::Node &node,
        const std::filesystem::path &pwd) {

        auto root = node;
        if (auto from_file_node = node["from-file"]; from_file_node) {
            auto path = pwd / from_file_node.as<std::string>();
            root = YAML::LoadFile(path);
        }

        if (auto def_radii_node = root["def-atom-radii"]; def_radii_node) {
            for (auto iter = def_radii_node.begin(); iter != def_radii_node.end(); ++iter) {
                def_atom_radii[iter->first.as<std::string>()] = iter->second.as<double>();
            }
        }

        if (auto amino_acids_node = root["amino-acids"]; amino_acids_node) {
            for (auto iter = amino_acids_node.begin(); iter != amino_acids_node.end(); ++iter) {
                auto aa = amino_acid(iter->first.as<std::string>());
                auto data_node = iter->second;

                amino_acid_data data;
                if (auto mass_node = data_node["mass"]; mass_node) {
                    data.mass = mass_node.as<double>();
                }
                if (auto alt_radii_node = data_node["alt-atom-radii"]; alt_radii_node) {
                    auto alt_atom_radii = alt_radii_node.as<std::map<std::string, double>>();
                    for (auto const& [atom, alt_radii]: alt_atom_radii) {
                        data.alt_atom_radii[atom] = alt_radii;
                    }
                }
                if (auto side_node = data_node["side"]; side_node) {
                    data.side_atoms = side_node.as<std::vector<std::string>>();
                }
                if (auto radius_node = data_node["radius"]; radius_node) {
                    data.radius = radius_node.as<double>();
                }
                if (auto ptype_node = data_node["polarization"]; ptype_node) {
                    polarization_type ptype;
                    double charge = 0.0;

                    auto ptype_name = ptype_node.as<std::string>();
                    if (ptype_name == "") {
                        ptype = NONE;
                    }
                    else if (ptype_name == "H") {
                        ptype = HYDROPHOBIC;
                    }
                    else if (ptype_name[0] == 'P') {
                        ptype = POLAR;
                        if (ptype_name == "P+") charge = 1.0;
                        else if (ptype_name == "P-") charge = -1.0;
                    }

                    data.ptype = ptype;
                    data.charge = charge;
                }
                if (auto cont_lim_node = data_node["contact-limits"]; cont_lim_node) {
                    contact_limits cont_lim;
                    cont_lim.back = (char)cont_lim_node["back"].as<short>();
                    cont_lim.side = (char)cont_lim_node["side"].as<short>();
                    cont_lim.side_p = (char)cont_lim_node["side-polar"].as<short>();
                    cont_lim.side_h = (char)cont_lim_node["side-hydrophobic"].as<short>();
                    data.cont_lim = cont_lim;
                }
            }
        }
    }

    void params::heurestic_angles::load_from(const YAML::Node &node,
        const std::filesystem::path &pwd) {

        xmd::csv_file csv;
        if (auto from_file_node = node["from-file"]; from_file_node) {
            auto path = pwd / from_file_node.as<std::string>();
            csv = csv_file(std::ifstream(path));
        }
        else {
            csv = csv_file(node.as<std::string>());
        }

        for (auto const& record: csv.records) {
            for (size_t i = 0; i < 6; ++i) {
                coeffs[record["type"]][i] = std::stod(record[i+1]);
            }
        }
    }

    void params::heurestic_dihedrals::load_from(const YAML::Node &node,
        const std::filesystem::path &pwd) {

        xmd::csv_file csv;
        if (auto from_file_node = node["from-file"]; from_file_node) {
            auto path = pwd / from_file_node.as<std::string>();
            csv = csv_file(std::ifstream(path));
        }
        else {
            csv = csv_file(node.as<std::string>());
        }

        for (auto const& record: csv.records) {
            coeffs_t c;
            c.for_sin = std::stod(record["sin"]);
            c.for_cos = std::stod(record["cos"]);
            c.for_sin2 = std::stod(record["sin2"]);
            c.for_cos2 = std::stod(record["cos2"]);
            c.for_sin_cos = std::stod(record["sin_cos"]);
            coeffs[record["type"]] = c;
        }
    }

    param_file::param_file(const std::filesystem::path &pf_path) {
        auto root = YAML::LoadFile(pf_path);
        auto pwd = pf_path.parent_path();
        load_from(root, pwd);
    }

    void param_file::load_from(const YAML::Node &node,
        const std::filesystem::path &pwd) {

        auto param_node = node["parameters"];
        if (auto inherit_node = param_node["inherit"]; inherit_node) {
            for (auto iter = inherit_node.begin(); iter != inherit_node.end(); ++iter) {
                auto path = pwd / iter->second.as<std::string>();
                auto file = YAML::LoadFile(path);
                load_from(file, path.parent_path());
            }

            if (auto override_node = param_node["override"]; override_node) {
                load_from(override_node, pwd);
            }
        }
        else {
            if (auto lj_node = param_node["lj"]; lj_node) {
                lj.load_from(lj_node, pwd);
            }
            if (auto amino_acids_node = param_node["amino-acids"]; amino_acids_node) {
                amino_acids.load_from(amino_acids_node, pwd);
            }
            if (auto heur_angles_node = param_node["heurestic-angles"]; heur_angles_node) {
                heurestic_angles.load_from(heur_angles_node, pwd);
            }
            if (auto heur_dih_node = param_node["heurestic-dihedrals"]; heur_dih_node) {
                heurestic_dihedrals.load_from(heur_dih_node, pwd);
            }
        }
    }


}