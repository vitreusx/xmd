#pragma once
#include <xmd/params/yaml_fs_node.h>
#include <xmd/types/scalar.h>
#include <variant>
#include <optional>
#include <array>
#include <xmd/types/amino_acid.h>
#include <unordered_map>
#include <xmd/utils/geometry.h>
#include <xmd/forces/primitives/lj_variants.h>
#include <filesystem>

namespace xmd {
    struct general_params {
        general_params() = default;
        explicit general_params(yaml_fs_node const &p);

        real total_time;
        uint64_t seed;
    };

    struct model_params {
        model_params() = default;
        explicit model_params(yaml_fs_node const &p);

        struct seqfile_params {
            seqfile_params() = default;
            explicit seqfile_params(yaml_fs_node const &p);

            std::string path;
        };

        struct pdbfile_params {
            pdbfile_params() = default;
            explicit pdbfile_params(yaml_fs_node const &p);

            std::string path;
        };

        std::variant<std::monostate, seqfile_params, pdbfile_params> file;

        struct morph_into_saw_params {
            morph_into_saw_params() = default;
            explicit morph_into_saw_params(yaml_fs_node const &p);

            std::optional<real> bond_distance;
            real residue_density;
            bool infer_box;
        };

        std::optional<morph_into_saw_params> morph_into_saw;
    };

    struct langevin_params {
        langevin_params() = default;
        explicit langevin_params(yaml_fs_node const &p);

        bool enabled;
        real gamma_factor, temperature;
    };

    struct integrator_params {
        integrator_params() = default;
        explicit integrator_params(yaml_fs_node const &p);

        bool enabled;
        real dt;
    };

    struct neighbor_list_params {
        neighbor_list_params() = default;
        explicit neighbor_list_params(yaml_fs_node const &p);

        real pad_factor;
    };

    struct heur_pair_res_type {
        heur_pair_res_type() = default;
        explicit heur_pair_res_type(char c);
        short value;
    };

    struct heur_coeff_pair_type {
        heur_coeff_pair_type() = default;
        heur_coeff_pair_type(heur_pair_res_type res1, heur_pair_res_type res2);
        short value ;
    };
}

namespace std {
    template<>
    struct hash<xmd::heur_coeff_pair_type> {
        uint64_t operator()(xmd::heur_coeff_pair_type const& pt) const;
    };
}

namespace xmd {
    struct heurestic_angles_params {
        heurestic_angles_params() = default;
        explicit heurestic_angles_params(yaml_fs_node const& p);
        bool enabled;

        using coeffs_for_pair = std::array<real, 6>;
        std::unordered_map<heur_coeff_pair_type, coeffs_for_pair> coeffs;
    };

    struct native_angles_params {
        native_angles_params() = default;
        explicit native_angles_params(yaml_fs_node const& p);
        bool enabled;
        real k;
    };

    struct cnd_params {
        cnd_params() = default;
        explicit cnd_params(yaml_fs_node const& p);
        bool enabled;
        real CDA, CDB;
    };

    struct snd_params {
        snd_params() = default;
        explicit snd_params(yaml_fs_node const& p);
        bool enabled;
        real CDH;
    };

    struct heurestic_dihedrals_params {
        heurestic_dihedrals_params() = default;
        explicit heurestic_dihedrals_params(yaml_fs_node const& p);
        bool enabled;

        struct coeffs_for_pair {
            real sin, cos, sin2, cos2, sin_cos;
        };
        std::unordered_map<heur_coeff_pair_type, coeffs_for_pair> coeffs;
    };

    struct const_es_params {
        const_es_params() = default;
        explicit const_es_params(yaml_fs_node const& p);
        bool enabled;
        real screening_distance, permittivity;
    };

    struct relative_es_params {
        relative_es_params() = default;
        explicit relative_es_params(yaml_fs_node const& p);
        bool enabled;
        real screening_distance, factor;
    };

    struct pid_params {
        pid_params() = default;
        explicit pid_params(yaml_fs_node const& p);
        bool enabled;

        enum class lambda_version_t {
            UNKNOWN, COSINE, ALGEBRAIC
        };
        lambda_version_t lambda_version;

        struct bb_params {
            bb_params() = default;
            explicit bb_params(yaml_fs_node const& p);
            real alpha, psi_0, r_min, depth;
        };
        bb_params bb_plus, bb_minus;

        struct ss_params {
            ss_params() = default;
            explicit ss_params(yaml_fs_node const& p);
            real alpha, psi_0;
        };
        ss_params ss;
    };

    struct qa_params {
        qa_params() = default;
        explicit qa_params(yaml_fs_node const& p);
        bool enabled;
        real phase_duration, breaking_factor, min_cos_hr, min_cos_hh, max_cos_nr;
    };

    struct chirality_params {
        chirality_params() = default;
        explicit chirality_params(yaml_fs_node const& p);
        bool enabled;
        real e_chi;
    };

    struct native_contacts_params {
        native_contacts_params() = default;
        explicit native_contacts_params(yaml_fs_node const& p);
        bool enabled;
        real lj_depth, active_threshold;
    };

    struct native_ssbonds_params {
        native_ssbonds_params() = default;
        explicit native_ssbonds_params(yaml_fs_node const& p);
        bool enabled;
        real H1, equil_dist;
    };

    struct pauli_params {
        pauli_params() = default;
        explicit pauli_params(yaml_fs_node const& p);
        bool enabled;
        real r_excl, depth;
    };

    struct tether_params {
        tether_params() = default;
        explicit tether_params(yaml_fs_node const& p);
        bool enabled;
        real H1, H2, def_length;
    };

    struct export_pdb_params {
        export_pdb_params() = default;
        explicit export_pdb_params(yaml_fs_node const& p);
        bool enabled;
        std::filesystem::path path;
        real exec_period;
    };

    struct progress_bar_params {
        progress_bar_params() = default;
        explicit progress_bar_params(yaml_fs_node const& p);
        bool enabled;
        int width;
        real update_period;
    };

    struct report_stats_params {
        report_stats_params() = default;
        explicit report_stats_params(yaml_fs_node const& p);
        bool enabled;
        std::filesystem::path path;
        real exec_period;
    };

    struct report_structure_params {
        report_structure_params() = default;
        explicit report_structure_params(yaml_fs_node const& p);
        bool enabled;
        std::filesystem::path root;
        std::string path_fmt;
        real exec_period;
    };

    struct solid_walls_params {
        solid_walls_params() = default;
        explicit solid_walls_params(yaml_fs_node const& p);
        bool enabled;
        real depth, cutoff;
        std::vector<plane> planes;
    };

    struct lj_attr_walls_params {
        lj_attr_walls_params() = default;
        explicit lj_attr_walls_params(yaml_fs_node const& p);
        bool enabled;
        real wall_min_dist, breaking_factor, phase_duration;
        std::vector<plane> planes;
    };

    struct velocity_afm_params {
        velocity_afm_params() = default;
        explicit velocity_afm_params(yaml_fs_node const& p);
        bool enabled;
        real H1, H2;

        struct afm_tip {
            int pulled_idx;
            std::optional<vec3r> origin;
            vec3r v;
        };
        std::vector<afm_tip> afm_tips;
    };

    struct force_afm_params {
        force_afm_params() = default;
        explicit force_afm_params(yaml_fs_node const& p);
        bool enabled;

        struct afm_tip {
            int pulled_idx;
            vec3r F;
        };
        std::vector<afm_tip> afm_tips;
    };

    struct checkpoints_params {
        checkpoints_params() = default;
        explicit checkpoints_params(yaml_fs_node const& p);
        bool enabled;
        real exec_period;
        std::filesystem::path root;
        std::string path_fmt;
    };

    struct freeform_params {
        freeform_params() = default;
        explicit freeform_params(yaml_fs_node const& p);

        general_params general;
        amino_acid_data aa_data;
        model_params model;
        langevin_params langevin;
        integrator_params integrator;
        neighbor_list_params nl;
        heurestic_angles_params heur_ang;
        native_angles_params nat_ang;
        cnd_params cnd;
        snd_params snd;
        heurestic_dihedrals_params heur_dih;
        const_es_params const_es;
        relative_es_params rel_es;
        lj_variants lj_vars;
        pid_params pid;
        qa_params qa;
        chirality_params chir;
        native_contacts_params nat_cont;
        native_ssbonds_params nat_ss;
        pauli_params pauli;
        tether_params tether;
        export_pdb_params export_pdb;
        progress_bar_params progress_bar;
        report_stats_params report_stats;
        report_structure_params report_structure;
        solid_walls_params solid_walls;
        lj_attr_walls_params lj_attr_walls;
        velocity_afm_params vel_afm;
        force_afm_params force_afm;
        checkpoints_params chkpt;
    };
}

