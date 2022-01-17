#include "freeform/params.h"
#include "utils/units.h"
#include "files/csv.h"

namespace xmd {

    general_params::general_params(const yaml_fs_node &p) {
        total_time = p["total time"].as<quantity>();
        seed = p["seed"].as<int>();
    }

    model_params::model_params(const yaml_fs_node &p) {
        if (auto seqfile_p = p["seqfile"]; seqfile_p) {
            file = seqfile_params(seqfile_p);
        }
        else if (auto pdbfile_p = p["pdb"]; pdbfile_p) {
            file = pdbfile_params(pdbfile_p);
        }

        if (auto morph_p = p["morph into SAW"]; morph_p) {
            morph_into_saw = morph_into_saw_params(morph_p);
        }
    }

    model_params::seqfile_params::seqfile_params(const yaml_fs_node &p) {
        path = p.as<std::string>();
    }

    model_params::pdbfile_params::pdbfile_params(const yaml_fs_node &p) {
        path = p.as<std::string>();
    }

    model_params::morph_into_saw_params::morph_into_saw_params(
        const yaml_fs_node &p) {
        bond_distance = p["bond distance"].as<quantity>();
        residue_density = p["residue density"].as<quantity>();
        infer_box = p["infer box"].as<bool>();
    }

    langevin_params::langevin_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        gamma_factor = p["gamma factor"].as<quantity>();
        temperature = p["temperature"].as<quantity>();
    }

    integrator_params::integrator_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        dt = p["dt"].as<quantity>();
    }

    neighbor_list_params::neighbor_list_params(const yaml_fs_node &p) {
        pad_factor = p["pad factor"].as<quantity>();
    }

    heur_pair_res_type::heur_pair_res_type(char c) {
        if (c == 'G') value = 0;
        else if (c == 'P') value = 1;
        else value = 2;
    }

    heur_coeff_pair_type::heur_coeff_pair_type(heur_pair_res_type res1,
        heur_pair_res_type res2) {
        value = 3 * res1.value + res2.value;
    }

    heurestic_angles_params::heurestic_angles_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        auto coeffs_csv = p["coefficients"].as<csv_file>();
        for (auto const& r: coeffs_csv.records) {
            auto type1 = heur_pair_res_type(r["type1"][0]);
            auto type2 = heur_pair_res_type(r["type2"][0]);
            auto pair = heur_coeff_pair_type(type1, type2);

            auto& cur_coeffs = coeffs[pair];
            for (int i = 0; i < 6; ++i) {
                auto colname ="a" + std::to_string(i);
                cur_coeffs[i] = std::stod(r[colname]);
            }
        }
    }

    native_angles_params::native_angles_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        k = p["k"].as<quantity>();
    }

    cnd_params::cnd_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        CDA = p["CDA"].as<quantity>();
        CDB = p["CDB"].as<quantity>();
    }

    snd_params::snd_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        CDH = p["CDH"].as<quantity>();
    }

    heurestic_dihedrals_params::heurestic_dihedrals_params(
        const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();

        auto coeffs_csv = p["coefficients"].as<csv_file>();
        for (auto const& r: coeffs_csv.records) {
            auto type2 = heur_pair_res_type(r["type2"][0]);
            auto type3 = heur_pair_res_type(r["type3"][0]);
            auto pair = heur_coeff_pair_type(type2, type3);

            auto& cur_coeffs = coeffs[pair];
            cur_coeffs.sin = quantity(r["sin"], eps);
            cur_coeffs.cos = quantity(r["cos"], eps);
            cur_coeffs.sin2 = quantity(r["sin2"], eps);
            cur_coeffs.cos2 = quantity(r["cos2"], eps);
            cur_coeffs.sin_cos = quantity(r["sin_cos"], eps);
        }
    }

    const_es_params::const_es_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        screening_distance = p["screening distance"].as<quantity>();
        permittivity = p["permittivity"].as<quantity>();
    }

    relative_es_params::relative_es_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        screening_distance = p["screening distance"].as<quantity>();
        factor = p["factor"].as<quantity>();
    }

    pid_params::pid_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();

        auto lambda_version_s = p["lambda version"].as<std::string>();
        if (lambda_version_s == "cosine")
            lambda_version = lambda_version_t::COSINE;
        else if (lambda_version_s == "algebraic")
            lambda_version = lambda_version_t::ALGEBRAIC;
        else
            lambda_version = lambda_version_t::UNKNOWN;

        bb_plus = bb_params(p["bb+"]);
        bb_minus = bb_params(p["bb-"]);
        ss = ss_params(p["ss"]);
    }

    pid_params::bb_params::bb_params(const yaml_fs_node &p) {
        alpha = p["alpha"].as<quantity>();
        psi_0 = p["psi_0"].as<quantity>();
        r_min = p["r_min"].as<quantity>();
        depth = p["depth"].as<quantity>();
    }

    pid_params::ss_params::ss_params(const yaml_fs_node &p) {
        alpha = p["alpha"].as<quantity>();
        psi_0 = p["psi_0"].as<quantity>();
    }

    qa_params::qa_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        phase_duration = p["phase duration"].as<quantity>();
        breaking_factor = p["breaking factor"].as<real>();
        min_cos_hr = p["min |cos(h, r)|"].as<real>();
        min_cos_hh = p["min |cos(h, h)| for bb"].as<real>();
        max_cos_nr = p["max cos(n, r)"].as<real>();
    }

    chirality_params::chirality_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        e_chi = p["e_chi"].as<quantity>();
    }

    native_contacts_params::native_contacts_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        lj_depth = p["lj depth"].as<quantity>();
        active_threshold = p["active threshold"].as<real>();
    }

    native_ssbonds_params::native_ssbonds_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        H1 = p["H1"].as<quantity>();
        equil_dist = p["equilibrium dist"].as<quantity>();
    }

    pauli_params::pauli_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        r_excl = p["r_excl"].as<quantity>();
        depth = p["depth"].as<quantity>();
    }

    tether_params::tether_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        H1 = p["H1"].as<quantity>();
        H2 = p["H2"].as<quantity>();
        def_length = p["default bond length"].as<quantity>();
    }

    export_pdb_params::export_pdb_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        path = p["path"].as<std::string>();
        exec_period = p["exec period"].as<quantity>();
    }

    progress_bar_params::progress_bar_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        width = p["width"].as<int>();
        update_period = p["update period"].as<quantity>();
    }

    report_stats_params::report_stats_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        path = p["path"].as<std::string>();
        exec_period = p["exec period"].as<quantity>();
    }

    report_structure_params::report_structure_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        path_fmt = p["path format"].as<std::string>();
        exec_period = p["exec period"].as<quantity>();
    }

    solid_walls_params::solid_walls_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        depth = p["depth"].as<quantity>();
        cutoff = p["cutoff"].as<quantity>();

        for (auto const& plane_p: p["planes"]) {
            auto normal = plane_p["normal"].as<vec3r>();
            auto origin = plane_p["origin"].as<vec3r>();
            planes.push_back(plane(origin, normal));
        }
    }

    lj_attr_walls_params::lj_attr_walls_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        wall_min_dist = p["wall min dist"].as<quantity>();
        breaking_factor = p["breaking factor"].as<real>();
        phase_duration = p["phase duration"].as<quantity>();

        for (auto const& plane_p: p["planes"]) {
            auto normal = plane_p["normal"].as<vec3r>();
            auto origin = plane_p["origin"].as<vec3r>();
            planes.push_back(plane(origin, normal));
        }
    }

    velocity_afm_params::velocity_afm_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        H1 = p["H1"].as<quantity>();
        H2 = p["H2"].as<quantity>();

        for (auto const& afm_p: p["AFM tips"]) {
            auto& tip = afm_tips.emplace_back();
            tip.pulled_idx = afm_p["pulled idx"].as<int>();
            if (auto origin_p = afm_p["origin"]; origin_p)
                tip.origin = origin_p.as<vec3r>();
            tip.v = afm_p["v"].as<vec3r>();
        }
    }

    force_afm_params::force_afm_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();

        for (auto const& afm_p: p["AFM tips"]) {
            auto& tip = afm_tips.emplace_back();
            tip.pulled_idx = afm_p["pulled idx"].as<int>();
            tip.F = afm_p["F"].as<vec3r>();
        }
    }

    checkpoints_params::checkpoints_params(const yaml_fs_node &p) {
        enabled = p["enabled"].as<bool>();
        exec_period = p["exec period"].as<quantity>();
        path_fmt = p["path format"].as<std::string>();
    }

    freeform_params::freeform_params(const yaml_fs_node &p) {
        general = general_params(p["general"]);
        model = model_params(p["model"]);
        aa_data = amino_acid_data(p["amino acid data"]);
        langevin = langevin_params(p["langevin"]);
        integrator = integrator_params(p["integrator"]);
        nl = neighbor_list_params(p["neighbor list"]);
        heur_ang = heurestic_angles_params(p["heurestic angles"]);
        nat_ang = native_angles_params(p["native angles"]);
        cnd = cnd_params(p["complex native dihedrals"]);
        snd = snd_params(p["simple native dihedrals"]);
        heur_dih = heurestic_dihedrals_params(p["heurestic dihedrals"]);
        const_es = const_es_params(p["const ES"]);
        rel_es = relative_es_params(p["relative ES"]);
        lj_vars = lj_variants(p["lj force variants"]);
        pid = pid_params(p["pseudo-improper dihedral potential"]);
        qa = qa_params(p["quasi-adiabatic potential"]);
        chir = chirality_params(p["chirality"]);
        nat_cont = native_contacts_params(p["native contacts"]);
        nat_ss = native_ssbonds_params(p["native ssbonds"]);
        pauli = pauli_params(p["Pauli exclusion"]);
        tether = tether_params(p["tether forces"]);
        export_pdb = export_pdb_params(p["export pdb"]);
        progress_bar = progress_bar_params(p["progress bar"]);
        report_stats = report_stats_params(p["report stats"]);
        report_structure = report_structure_params(p["report structure"]);
        solid_walls = solid_walls_params(p["solid walls"]);
        lj_attr_walls = lj_attr_walls_params(p["LJ attractive walls"]);
        vel_afm = velocity_afm_params(p["velocity AFM"]);
        force_afm = force_afm_params(p["force AFM"]);
        chkpt = checkpoints_params(p["checkpoints"]);
    }
}

namespace std {
    uint64_t hash<xmd::heur_coeff_pair_type>::operator()(
        const xmd::heur_coeff_pair_type &pt) const {
        return std::hash<short>()(pt.value);
    }
}