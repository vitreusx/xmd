#pragma once
#include <string>
#include "config.h"

namespace fastmd {
    enum class amino_acid_code: char {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    class amino_acid {
    public:
        amino_acid(amino_acid_code code);
        amino_acid(char letter);
        amino_acid(std::string const& name);

        amino_acid_code get_code() const;
        char get_letter() const;
        std::string const& get_name() const;

        static constexpr int TOTAL = 20;
    };
}