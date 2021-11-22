#pragma once
#include "xmd/types/amino_acid.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <optional>
#include <array>

namespace xmd::params {
    struct specific_lj {
        double def_sigma, def_depth;

        using value_for_aa = std::array<double, amino_acid::NUM_AA>;
        using value_for_pair = std::array<value_for_aa, amino_acid::NUM_AA>;
        std::optional<value_for_pair> pairwise_sigma, pairwise_depth;

        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
    };

    struct lj {
        specific_lj bb, bs, ss;

        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
    };

    struct contact_limits {
        char back, side, side_h, side_p;
    };

    struct amino_acid_data {
        double mass, radius, charge;
        std::vector<std::string> side_atoms;
        std::unordered_map<std::string, double> alt_atom_radii;
        polarization_type ptype;
        contact_limits cont_lim;
    };

    struct amino_acids {
        std::unordered_map<std::string, double> def_atom_radii;
        std::unordered_map<xmd::amino_acid, amino_acid_data> data;

        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
        double atom_radius(xmd::amino_acid res, std::string const& atom) const;
    };

    struct heurestic_angles {
        std::unordered_map<std::string, double[6]> coeffs;

        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
    };

    struct heurestic_dihedrals {
        struct coeffs_t {
            double for_sin, for_cos, for_sin2, for_cos2, for_sin_cos;
        };
        std::unordered_map<std::string, coeffs_t> coeffs;

        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
    };
}

namespace xmd {
    class param_file {
    public:
        void load_from(YAML::Node const& node, std::filesystem::path const& pwd);
        explicit param_file(std::filesystem::path const& pf_path);

        params::lj lj;
        params::amino_acids amino_acids;
        params::heurestic_angles heurestic_angles;
        params::heurestic_dihedrals heurestic_dihedrals;
    };
}