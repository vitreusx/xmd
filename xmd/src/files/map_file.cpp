#include "map_file.h"
#include <fstream>
#include <sstream>
#include "files/csv.h"

namespace xmd {
    void map_file::shift(int shift_val) {
        for (auto& cont: contacts) {
            cont.i1 += shift_val;
            cont.i2 += shift_val;
        }

        for (auto& ang: angles) {
            ang.i1 += shift_val;
            ang.i2 += shift_val;
            ang.i3 += shift_val;
        }

        for (auto& dih: dihedrals) {
            dih.i1 += shift_val;
            dih.i2 += shift_val;
            dih.i3 += shift_val;
            dih.i4 += shift_val;
        }
    }
}

namespace YAML {
    Node convert<xmd::map_file>::encode(const xmd::map_file &mf) {
        Node node;

        auto contacts_csv = xmd::csv_file();
        contacts_csv.set_header({"i1", "i2", "length"});
        for (auto const& cont: mf.contacts) {
            auto& record = contacts_csv.add_record();
            record["i1"] = std::to_string(cont.i1);
            record["i2"] = std::to_string(cont.i2);
            record["length"] = std::to_string(cont.length);
        }
        node["contacts"] = contacts_csv.print();

        auto angles_csv = xmd::csv_file();
        angles_csv.set_header({"i1", "i2", "i3", "theta"});
        for (auto const& angle: mf.angles) {
            auto& record = contacts_csv.add_record();
            record["i1"] = std::to_string(angle.i1);
            record["i2"] = std::to_string(angle.i2);
            record["i3"] = std::to_string(angle.i3);
            record["theta"] = std::to_string(angle.theta);
        }
        node["angles"] = angles_csv.print();

        auto dihedrals_csv = xmd::csv_file();
        dihedrals_csv.set_header({"i1", "i2", "i3", "i4", "phi"});
        for (auto const& dihedral: mf.dihedrals) {
            auto& record = contacts_csv.add_record();
            record["i1"] = std::to_string(dihedral.i1);
            record["i2"] = std::to_string(dihedral.i2);
            record["i3"] = std::to_string(dihedral.i3);
            record["i4"] = std::to_string(dihedral.i4);
            record["phi"] = std::to_string(dihedral.phi);
        }
        node["dihedrals"] = dihedrals_csv.print();

        return node;
    }

    bool convert<xmd::map_file>::decode(const Node &node, xmd::map_file &mf) {
        auto fetch_csv = [](auto const& node) -> auto {
            if (node.IsMap() && node["from-file"]) {
                auto filename = node["from-file"].template as<std::string>();
                std::ifstream csv_file(filename);
                return xmd::csv_file(std::ifstream(filename));
            }
            else  {
                auto text = node.template as<std::string>();
                return xmd::csv_file(text);
            }
        };

        auto contacts_csv = fetch_csv(node["contacts"]);
        for (auto const& record: contacts_csv.records) {
            xmd::map_file::contact contact = {};
            contact.i1 = std::stoi(record["i1"]);
            contact.i2 = std::stoi(record["i2"]);
            contact.length = std::stod(record["length"]);
            mf.contacts.push_back(contact);
        }

        auto angles_csv = fetch_csv(node["angles"]);
        for (auto const& record: angles_csv.records) {
            xmd::map_file::angle angle = {};
            angle.i1 = std::stoi(record["i1"]);
            angle.i2 = std::stoi(record["i2"]);
            angle.i3 = std::stoi(record["i3"]);
            angle.theta = std::stod(record["theta"]);
            mf.angles.push_back(angle);
        }

        auto dihedrals_csv = fetch_csv(node["dihedrals"]);
        for (auto const& record: dihedrals_csv.records) {
            xmd::map_file::dihedral dihedral = {};
            dihedral.i1 = std::stoi(record["i1"]);
            dihedral.i2 = std::stoi(record["i2"]);
            dihedral.i3 = std::stoi(record["i3"]);
            dihedral.i4 = std::stoi(record["i4"]);
            dihedral.phi = std::stod(record["phi"]);
            mf.dihedrals.push_back(dihedral);
        }

        return true;
    }
}