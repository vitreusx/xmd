#include "map_file.h"
#include <fstream>
#include <sstream>
#include <files/csv.h>
#include <yaml-cpp/yaml.h>

namespace xmd {
    static std::shared_ptr<std::istream> data_stream(YAML::Node const& node) {
        if (node.IsMap() && node["from-file"]) {
            auto filename = node["from-file"].as<std::string>();
            return std::make_shared<std::ifstream>(filename);
        }
        else if (node.IsScalar()) {
            auto text = node.as<std::string>();
            return std::make_shared<std::stringstream>(text);
        }
        else {
            return nullptr;
        }
    }

    map_file::map_file(std::filesystem::path const& path) {
        auto yaml_file = YAML::LoadFile(path);

        auto contacts_node = yaml_file["contacts"];
        if (contacts_node) {
            auto contacts_csv_source = data_stream(contacts_node);
            auto contacts_csv = csv_file(*contacts_csv_source);

            for (auto const& cont_record: contacts_csv) {
                map_file::contact contact = {};
                contact.i1 = std::stoi(cont_record["i1"]);
                contact.i2 = std::stoi(cont_record["i2"]);
                contact.d = std::stof(cont_record["d"]);
                contacts.push_back(contact);
            }
        }

        auto angles_node = yaml_file["angles"];
        if (angles_node) {
            auto angles_csv_source = data_stream(angles_node);
            auto angles_csv = csv_file(*angles_csv_source);

            for (auto const& angle_record: angles_csv) {
                map_file::angle angle = {};
                angle.i1 = std::stoi(angle_record["i1"]);
                angle.i2 = std::stoi(angle_record["i2"]);
                angle.i3 = std::stoi(angle_record["i3"]);
                angle.theta = std::stof(angle_record["theta"]);
                angles.push_back(angle);
            }
        }

        auto dihedrals_node = yaml_file["dihedrals"];
        if (dihedrals_node) {
            auto dihedrals_csv_source = data_stream(dihedrals_node);
            auto dihedrals_csv = csv_file(*dihedrals_csv_source);

            for (auto const& dihedral_record: dihedrals_csv) {
                map_file::dihedral dihedral = {};
                dihedral.i1 = std::stoi(dihedral_record["i1"]);
                dihedral.i2 = std::stoi(dihedral_record["i2"]);
                dihedral.i3 = std::stoi(dihedral_record["i3"]);
                dihedral.i4 = std::stoi(dihedral_record["i4"]);
                dihedral.phi = std::stof(dihedral_record["phi"]);
                dihedrals.push_back(dihedral);
            }
        }
    }
}
