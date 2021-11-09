#pragma once
#include <string>
#include "config.h"

namespace fastmd {
    enum amino_acid_code: char {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    class amino_acid {
    public:
        ANY inline explicit amino_acid(amino_acid_code code);
        ANY inline explicit amino_acid(char letter);
        ANY inline explicit amino_acid(std::string const& name);

        ANY inline operator amino_acid_code() const;
        ANY inline char get_letter() const;
        ANY inline std::string const& get_name() const;

        static constexpr int TOTAL = 20;

    private:
        amino_acid_code code;
    };
}

namespace std {
    template<>
    struct hash<fastmd::amino_acid> {
        size_t operator()(fastmd::amino_acid const& aa) const {
            return hash<fastmd::amino_acid_code>()((fastmd::amino_acid_code)aa);
        }
    };
}

#include "amino_acid.inl"
