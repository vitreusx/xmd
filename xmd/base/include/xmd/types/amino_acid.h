#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace xmd {
    enum aa_code: char {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    enum polarization_type: char {
        NONE, POLAR, HYDROPHOBIC
    };

    class amino_acid {
    public:
        constexpr amino_acid() = default;

        inline constexpr explicit amino_acid(aa_code code):
            code{code} {};

        inline explicit amino_acid(char letter) {
            static const std::unordered_map<char, aa_code> letter_to_code = {
                { 'A', ALA }, { 'R', ARG }, { 'N', ASN }, { 'D', ASP },
                { 'C', CYS }, { 'E', GLU }, { 'Q', GLN }, { 'G', GLY },
                { 'H', HIS }, { 'I', ILE }, { 'L', LEU }, { 'K', LYS },
                { 'M', MET }, { 'F', PHE }, { 'P', PRO }, { 'S', SER },
                { 'T', THR }, { 'W', TRP }, { 'Y', TYR }, { 'V', VAL }
            };

            code = letter_to_code.at(letter);
        }

        inline explicit amino_acid(std::string const& name) {
            static const std::unordered_map<std::string, aa_code> name_to_code = {
                { "ALA", ALA }, { "ARG", ARG }, { "ASN", ASN }, { "ASP", ASP },
                { "CYS", CYS }, { "GLU", GLU }, { "GLN", GLN }, { "GLY", GLY },
                { "HIS", HIS }, { "ILE", ILE }, { "LEU", LEU }, { "LYS", LYS },
                { "MET", MET }, { "PHE", PHE }, { "PRO", PRO }, { "SER", SER },
                { "THR", THR }, { "TRP", TRP }, { "TYR", TYR }, { "VAL", VAL }
            };

            code = name_to_code.at(name);
        }

        inline constexpr operator aa_code() const {
            return code;
        }

        inline char letter() const {
            static const std::unordered_map<aa_code, char> code_to_letter = {
                { ALA, 'A' }, { ARG, 'R' }, { ASN, 'N' }, { ASP, 'D' },
                { CYS, 'C' }, { GLU, 'E' }, { GLN, 'Q' }, { GLY, 'G' },
                { HIS, 'H' }, { ILE, 'I' }, { LEU, 'L' }, { LYS, 'K' },
                { MET, 'M' }, { PHE, 'F' }, { PRO, 'P' }, { SER, 'S' },
                { THR, 'T' }, { TRP, 'W' }, { TYR, 'Y' }, { VAL, 'V' }
            };

            return code_to_letter.at(code);
        }

        inline std::string const& name() const {
            static const std::unordered_map<aa_code, std::string> code_to_name = {
                { ALA, "ALA" }, { ARG, "ARG" }, { ASN, "ASN" }, { ASP, "ASP" },
                { CYS, "CYS" }, { GLU, "GLU" }, { GLN, "GLN" }, { GLY, "GLY" },
                { HIS, "HIS" }, { ILE, "ILE" }, { LEU, "LEU" }, { LYS, "LYS" },
                { MET, "MET" }, { PHE, "PHE" }, { PRO, "PRO" }, { SER, "SER" },
                { THR, "THR" }, { TRP, "TRP" }, { TYR, "TYR" }, { VAL, "VAL" }
            };

            return code_to_name.at(code);
        }

        static inline constexpr int NUM_AA = 20;

    private:
        aa_code code = ALA;
    };
}

namespace std {
    template<>
    struct hash<xmd::amino_acid> {
        size_t operator()(xmd::amino_acid const& aa) const {
            return hash<xmd::aa_code>()((xmd::aa_code)aa);
        }
    };
}
