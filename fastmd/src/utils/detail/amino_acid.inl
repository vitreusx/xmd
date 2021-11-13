#pragma once
#include <unordered_map>

namespace fastmd {
    constexpr amino_acid::amino_acid(aa_code code) :
        code{code} {}

    amino_acid::amino_acid(char letter) {
        static const std::unordered_map<char, aa_code> letter_to_code = {
            { 'A', ALA }, { 'R', ARG }, { 'N', ASN }, { 'D', ASP },
            { 'C', CYS }, { 'E', GLU }, { 'Q', GLN }, { 'G', GLY },
            { 'H', HIS }, { 'I', ILE }, { 'L', LEU }, { 'K', LYS },
            { 'M', MET }, { 'F', PHE }, { 'P', PRO }, { 'S', SER },
            { 'T', THR }, { 'W', TRP }, { 'Y', TYR }, { 'V', VAL }
        };
        
        code = letter_to_code[letter];
    }

    amino_acid::amino_acid(const std::string &name) {
        static const std::unordered_map<std::string, aa_code> name_to_code = {
            { "ALA", ALA }, { "ARG", ARG }, { "ASN", ASN }, { "ASP", ASP },
            { "CYS", CYS }, { "GLU", GLU }, { "GLN", GLN }, { "GLY", GLY },
            { "HIS", HIS }, { "ILE", ILE }, { "LEU", LEU }, { "LYS", LYS },
            { "MET", MET }, { "PHE", PHE }, { "PRO", PRO }, { "SER", SER },
            { "THR", THR }, { "TRP", TRP }, { "TYR", TYR }, { "VAL", VAL }
        };

        code = name_to_code[name];
    }

    constexpr amino_acid::operator aa_code() const {
        return code;
    }

    char amino_acid::letter() const {
        static const std::unordered_map<aa_code, char> code_to_letter = {
            { ALA, 'A' }, { ARG, 'R' }, { ASN, 'N' }, { ASP, 'D' },
            { CYS, 'C' }, { GLU, 'E' }, { GLN, 'Q' }, { GLY, 'G' },
            { HIS, 'H' }, { ILE, 'I' }, { LEU, 'L' }, { LYS, 'K' },
            { MET, 'M' }, { PHE, 'F' }, { PRO, 'P' }, { SER, 'S' },
            { THR, 'T' }, { TRP, 'W' }, { TYR, 'Y' }, { VAL, 'V' }
        };
        
        return code_to_letter[code];
    }

    std::string const& amino_acid::name() const {
        static const std::unordered_map<aa_code, std::string> code_to_name = {
            { ALA, "ALA" }, { ARG, "ARG" }, { ASN, "ASN" }, { ASP, "ASP" },
            { CYS, "CYS" }, { GLU, "GLU" }, { GLN, "GLN" }, { GLY, "GLY" },
            { HIS, "HIS" }, { ILE, "ILE" }, { LEU, "LEU" }, { LYS, "LYS" },
            { MET, "MET" }, { PHE, "PHE" }, { PRO, "PRO" }, { SER, "SER" },
            { THR, "THR" }, { TRP, "TRP" }, { TYR, "TYR" }, { VAL, "VAL" }
        };

        return code_to_name[code];
    }
}