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
        inline explicit amino_acid(amino_acid_code code);
        inline explicit amino_acid(char letter);
        inline explicit amino_acid(std::string const& name);

        inline operator amino_acid_code() const;
        inline char get_letter() const;
        inline std::string const& get_name() const;

        static constexpr int TOTAL = 20;

    private:
        amino_acid_code code;
    };
}

namespace std {
    template<>
    struct hash<fastmd::amino_acid> {
        size_t operator()(fastmd::amino_acid const& aa) const {
            return hash<fastmd::amino_acid_code>()(aa.get_code());
        }
    };
}

#include "amino_acid.inl"
