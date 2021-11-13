#pragma once
#include <string>
#include "config.h"

namespace fastmd {
    enum aa_code: char {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    enum polarization_type: char {
        NONE, POLAR, HYDROPHOBIC
    };

    class amino_acid {
    public:
        inline constexpr explicit amino_acid(aa_code code);
        inline explicit amino_acid(char letter);
        inline explicit amino_acid(std::string const& name);

        inline constexpr operator aa_code() const;
        inline char letter() const;
        inline std::string const& name() const;

        static inline constexpr int NUM_AA = 20;

    private:
        aa_code code;
    };
}

namespace std {
    template<>
    struct hash<fastmd::amino_acid> {
        size_t operator()(fastmd::amino_acid const& aa) const {
            return hash<fastmd::aa_code>()((fastmd::aa_code)aa);
        }
    };
}

#include "detail/amino_acid.inl"
