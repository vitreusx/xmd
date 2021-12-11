#include "types/amino_acid.h"
#include "utils/units.h"
#include <numeric>
#include <xmd/params/param_file.h>

namespace xmd {

    amino_acid::amino_acid(char letter) {
        static const std::unordered_map<char, aa_code> letter_to_code = {
            { 'A', ALA }, { 'R', ARG }, { 'N', ASN }, { 'D', ASP },
            { 'C', CYS }, { 'E', GLU }, { 'Q', GLN }, { 'G', GLY },
            { 'H', HIS }, { 'I', ILE }, { 'L', LEU }, { 'K', LYS },
            { 'M', MET }, { 'F', PHE }, { 'P', PRO }, { 'S', SER },
            { 'T', THR }, { 'W', TRP }, { 'Y', TYR }, { 'V', VAL }
        };

        code = letter_to_code.at(letter);
    }

    amino_acid::amino_acid(const std::string &name) {
        static const std::unordered_map<std::string, aa_code> name_to_code = {
            { "ALA", ALA }, { "ARG", ARG }, { "ASN", ASN }, { "ASP", ASP },
            { "CYS", CYS }, { "GLU", GLU }, { "GLN", GLN }, { "GLY", GLY },
            { "HIS", HIS }, { "ILE", ILE }, { "LEU", LEU }, { "LYS", LYS },
            { "MET", MET }, { "PHE", PHE }, { "PRO", PRO }, { "SER", SER },
            { "THR", THR }, { "TRP", TRP }, { "TYR", TYR }, { "VAL", VAL }
        };

        code = name_to_code.at(name);
    }

    char amino_acid::letter() const {
        static const std::unordered_map<aa_code, char> code_to_letter = {
            { ALA, 'A' }, { ARG, 'R' }, { ASN, 'N' }, { ASP, 'D' },
            { CYS, 'C' }, { GLU, 'E' }, { GLN, 'Q' }, { GLY, 'G' },
            { HIS, 'H' }, { ILE, 'I' }, { LEU, 'L' }, { LYS, 'K' },
            { MET, 'M' }, { PHE, 'F' }, { PRO, 'P' }, { SER, 'S' },
            { THR, 'T' }, { TRP, 'W' }, { TYR, 'Y' }, { VAL, 'V' }
        };

        return code_to_letter.at(code);
    }

    std::string const &amino_acid::name() const {
        static const std::unordered_map<aa_code, std::string> code_to_name = {
            { ALA, "ALA" }, { ARG, "ARG" }, { ASN, "ASN" }, { ASP, "ASP" },
            { CYS, "CYS" }, { GLU, "GLU" }, { GLN, "GLN" }, { GLY, "GLY" },
            { HIS, "HIS" }, { ILE, "ILE" }, { LEU, "LEU" }, { LYS, "LYS" },
            { MET, "MET" }, { PHE, "PHE" }, { PRO, "PRO" }, { SER, "SER" },
            { THR, "THR" }, { TRP, "TRP" }, { TYR, "TYR" }, { VAL, "VAL" }
        };

        return code_to_name.at(code);
    }

    std::vector<amino_acid> amino_acid::all() {
        static const std::vector<amino_acid> all_aminoacids = {
            amino_acid(ALA), amino_acid(ARG), amino_acid(ASN), amino_acid(ASP),
            amino_acid(CYS), amino_acid(GLU), amino_acid(GLN), amino_acid(GLY),
            amino_acid(HIS), amino_acid(ILE), amino_acid(LEU), amino_acid(LYS),
            amino_acid(MET), amino_acid(PHE), amino_acid(PRO), amino_acid(SER),
            amino_acid(THR), amino_acid(TRP), amino_acid(TYR), amino_acid(VAL)
        };

        return all_aminoacids;
    }

    amino_acid_data param_value_parser<amino_acid_data>::parse(
        const param_entry &root) const {

        auto source = root;
        if (auto from_file_tag = root["from file"]; from_file_tag) {
            source = param_file::import(root, from_file_tag.as<std::string>());
        }

        amino_acid_data parsed;

        std::unordered_map<std::string, true_real> def_atom_radii;
        auto def_atom_radii_node = source["default atom radii"];
        for (auto const& entry: source["default atom radii"]) {
            auto name = entry.first.as<std::string>();
            auto r = quantity(entry.second.as<std::string>(), angstrom);
            def_atom_radii[name] = r;
        }

        auto amino_acids_node = source["amino acids"];
        for (auto const& entry: source["amino acids"]) {
            auto name = entry.first.as<std::string>();
            auto data_node = entry.second;

            aa_data& res_data = parsed.data[amino_acid(name)];
            res_data.mass = quantity(data_node["mass"].as<std::string>(), amu);
            res_data.radius = quantity(data_node["radius"].as<std::string>(), angstrom);

            auto atom_radii = def_atom_radii;
            if (auto alt_radii_node = data_node["alt atom radii"]; alt_radii_node) {
                for (auto const& atom_entry: alt_radii_node) {
                    auto atom_name = atom_entry.first.as<std::string>();
                    auto r = quantity(atom_entry.second.as<std::string>(), angstrom);
                    atom_radii[atom_name] = r;
                }
            }

            for (auto const& back_atom: { "N", "CA", "C", "O", "OXT" }) {
                atom_data& atom_ = res_data.atoms[back_atom];
                atom_.name = back_atom;
                atom_.radius = atom_radii[back_atom];
                atom_.backbone = true;
            }

            if (auto side_atoms_node = data_node["side"]; side_atoms_node) {
                for (auto const& side_atom_node: side_atoms_node) {
                    auto side_atom = side_atom_node.as<std::string>();
                    atom_data& atom_ = res_data.atoms[side_atom];
                    atom_.name = side_atom;
                    atom_.radius = atom_radii[side_atom];
                    atom_.backbone = false;
                }
            }

            res_data.polarization = NONE;
            if (auto polarization_node = data_node["polarization"]; polarization_node) {
                auto ptype = polarization_node.as<std::string>();
                if (ptype == "polar") {
                    res_data.polarization = POLAR;
                }
                else if (ptype == "hydrophobic") {
                    res_data.polarization = HYDROPHOBIC;
                }
                else {
                    res_data.polarization = NONE;
                }
            }

            res_data.charge = 0.0;
            if (auto charge_node = data_node["charge"]; charge_node) {
                auto charge = quantity(charge_node.as<std::string>(), echarge);
                res_data.charge = charge;
            }

            res_data.limits = { 0, 0, 0, 0 };
            for (auto const& limit_entry: data_node["contact limits"]) {
                auto limit_name = limit_entry.first.as<std::string>();
                auto limit_val = limit_entry.second.as<int>();

                if (limit_name == "back") {
                    res_data.limits.back = limit_val;
                }
                else if (limit_name == "side (all)") {
                    res_data.limits.side_all = limit_val;
                }
                else if (limit_name == "side (hydrophobic)") {
                    res_data.limits.side_hydrophobic = limit_val;
                }
                else if (limit_name == "side (polar)") {
                    res_data.limits.side_polar = limit_val;
                }
            }
        }

        auto avg_res_mass = std::accumulate(parsed.data.begin(), parsed.data.end(), 0.0,
            [](auto const& sum, auto const& entry) -> auto {
                auto const& [name, res_data] = entry;
                return sum + res_data.mass;
            }) / (true_real)parsed.data.size();

        for (auto& [name, res_data]: parsed.data) {
            res_data.mass = (res_data.mass / avg_res_mass) * f77mass;
        }

        return parsed;
    }

    aa_data const &amino_acid_data::operator[](const amino_acid &aa) const {
        return data.at(aa);
    }
}