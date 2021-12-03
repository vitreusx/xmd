#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <xmd/utils/param_entry.h>

namespace xmd {
    enum aa_code : char {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    enum polarization_type : char {
        NONE, POLAR, HYDROPHOBIC
    };

    class amino_acid {
    public:
        constexpr amino_acid() = default;

        inline constexpr explicit amino_acid(aa_code code) :
            code{code} {};

        inline constexpr operator aa_code() const {
            return code;
        }

        explicit amino_acid(char letter);

        char letter() const;

        explicit amino_acid(std::string const &name);

        std::string const &name() const;

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

namespace xmd {
    struct atom_data {
        std::string name;
        double radius;
        bool backbone;
    };

    struct contact_limits {
        int back, side_all, side_hydrophobic, side_polar;
    };

    struct aa_data {
        double mass, radius;
        std::unordered_map<std::string, atom_data> atoms;
        polarization_type polarization;
        double charge;
        contact_limits limits;
    };

    class amino_acid_data: public param_entry {
    public:
        std::unordered_map<amino_acid, aa_data> data;

    public:
        void load_from_node(YAML::Node const& root,
            std::filesystem::path const& pwd) override;

        aa_data const& operator[](amino_acid const& aa) const;
    };
}


