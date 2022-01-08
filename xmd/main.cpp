#include <xmd/utils/units.h>
#include <xmd/ctx/context.h>
#include <xmd/model/loader.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/reset_thread_vf.h>
#include <xmd/dynamics/reduce_vf.h>
#include <xmd/dynamics/lang_pc.h>
#include <xmd/forces/all.h>
#include <xmd/io/show_progress_bar.h>
#include <xmd/io/export_pdb.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/nl/verify.h>
#include <xmd/io/report_stats.h>
#include <xmd/io/report_structure.h>
#include <xmd/nl/legacy_update.h>
#include <omp.h>
#include <iostream>
#include <string>
#include <xmd/params/defaults.h>
#include <xmd/utils/yaml.h>
#include <xmd/io/create_checkpoint.h>

using namespace xmd;

int main(int argc, char **argv) {
    omp_set_num_threads(2);
    std::string param_path = "data/examples/defaults.yml";

    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "--help") {
            std::cout << "Usage: " << argv[0] << " [inputfile]\n";
            return 0;
        }
        else {
            param_path = arg1;
        }
    }

    context def_ctx;
    auto &params = def_ctx.persistent<yaml_fs_node>("params",
        lazy([&]() -> auto {
            auto const& defaults = default_params();
            auto overrides = YAML::LoadFile(param_path);
            auto merged = merge_yaml(defaults, overrides);
            return yaml_fs_node(merged, param_path);
        }));

    def_ctx.ephemeral<real>("total_time",
        params["general"]["total time"].as<quantity>());
    def_ctx.ephemeral<amino_acid_data>("amino_acid_data",
        params["amino acid data"].as<amino_acid_data>());
    def_ctx.ephemeral<lj_variants>("lj_variants");
    auto& seed = def_ctx.ephemeral<int>("seed",
        params["general"]["seed"].as<int>());
    def_ctx.ephemeral<xmd::rand_gen>("gen", seed);

    model_loader().declare_vars(def_ctx);

#pragma omp parallel default(none) shared(def_ctx,params)
    {
        auto lang_pc_enabled = params["langevin"]["enabled"].as<bool>();
        lang_pc_step lang_pc_;
        if (lang_pc_enabled)
            lang_pc_.declare_vars(def_ctx);

        reset_vf reset_vf_;
        reset_vf_.declare_vars(def_ctx);

        reset_thread_vf reset_thread_vf_;
        reset_thread_vf_.declare_vars(def_ctx);

        reduce_vf reduce_vf_omp_;
        reduce_vf_omp_.declare_vars(def_ctx);

        auto export_pdb_enabled = params["export pdb"]["enabled"].as<bool>();
        export_pdb export_pdb_;
        if (export_pdb_enabled)
            export_pdb_.declare_vars(def_ctx);

        auto show_pbar_enabled = params["progress bar"]["enabled"].as<bool>();
        show_progress_bar show_pbar_;
        if (show_pbar_enabled)
            show_pbar_.declare_vars(def_ctx);

        auto report_stats_enabled = params["report stats"]["enabled"].as<bool>();
        report_stats report_stats_;
        if (report_stats_enabled)
            report_stats_.declare_vars(def_ctx);

        auto report_structure_enabled = params["report structure"]["enabled"].as<bool>();
        report_structure report_structure_;
        if (report_structure_enabled)
            report_structure_.declare_vars(def_ctx);

        auto chir_enabled = params["chirality"]["enabled"].as<bool>();
        eval_chiral_forces eval_chir_;
        if (chir_enabled)
            eval_chir_.declare_vars(def_ctx);

        auto tethers_enabled = params["tether forces"]["enabled"].as<bool>();
        eval_tether_forces tethers_;
        if (tethers_enabled)
            tethers_.declare_vars(def_ctx);

        auto nat_ang_enabled = params["native angles"]["enabled"].as<bool>();
        eval_native_angle_forces nat_ang_;
        if (nat_ang_enabled)
            nat_ang_.declare_vars(def_ctx);

        auto nat_comp_dih_enabled = params["complex native dihedrals"]["enabled"].as<bool>();
        eval_cnd_forces nat_comp_dih_;
        if (nat_comp_dih_enabled)
            nat_comp_dih_.declare_vars(def_ctx);

        auto nat_simp_dih_enabled = params["simple native dihedrals"]["enabled"].as<bool>();
        eval_snd_forces nat_simp_dih_;
        if (nat_simp_dih_enabled)
            nat_simp_dih_.declare_vars(def_ctx);

        nl::legacy_update legacy_update_;
        legacy_update_.declare_vars(def_ctx);
        nl::verify nl_verify_;
        nl_verify_.declare_vars(def_ctx);

        auto pauli_enabled = params["Pauli exclusion"]["enabled"].as<bool>();
        eval_pauli_exclusion_forces eval_pauli_;
        update_pauli_pairs update_pauli_;
        if (pauli_enabled) {
            eval_pauli_.declare_vars(def_ctx);
            update_pauli_.declare_vars(def_ctx);
        }

        auto go_enabled = params["native contacts"]["enabled"].as<bool>();
        eval_go_forces eval_go_;
        update_go_contacts update_go_;
        if (go_enabled) {
            eval_go_.declare_vars(def_ctx);
            update_go_.declare_vars(def_ctx);
        }

        auto ss_enabled = params["native ssbonds"]["enabled"].as<bool>();
        eval_nat_ssbond_forces eval_ss_;
        update_nat_ssbonds update_ss_;
        if (ss_enabled) {
            eval_ss_.declare_vars(def_ctx);
            update_ss_.declare_vars(def_ctx);
        }

        auto const_es_enabled = params["const ES"]["enabled"].as<bool>();
        eval_const_es_forces eval_const_es_;
        update_const_es update_const_es_;
        if (const_es_enabled) {
            eval_const_es_.declare_vars(def_ctx);
            update_const_es_.declare_vars(def_ctx);
        }

        auto rel_es_enabled = params["relative ES"]["enabled"].as<bool>();
        eval_relative_es_forces eval_rel_es_;
        update_relative_es update_rel_es_;
        if (rel_es_enabled) {
            eval_rel_es_.declare_vars(def_ctx);
            update_rel_es_.declare_vars(def_ctx);
        }

        auto qa_enabled = params["quasi-adiabatic"]["enabled"].as<bool>();
        qa::eval_qa_forces eval_qa_;
        qa::update_free_pairs update_qa_;
        if (qa_enabled) {
            eval_qa_.declare_vars(def_ctx);
            update_qa_.declare_vars(def_ctx);
        }

        auto pid_enabled = params["pseudo-improper dihedral"]["enabled"].as<bool>();
        pid::eval_pid_forces eval_pid_;
        pid::update_pid_bundles update_pid_;
        if (pid_enabled) {
            eval_pid_.declare_vars(def_ctx);
            update_pid_.declare_vars(def_ctx);
        }

        auto solid_enabled = params["solid walls"]["enabled"].as<bool>();
        eval_solid_wall_forces eval_solid_;
        if (solid_enabled)
            eval_solid_.declare_vars(def_ctx);

        auto lj_attr_enabled = params["LJ attractive walls"]["enabled"].as<bool>();
        eval_lj_attr_wall_forces eval_lj_attr_;
        if (lj_attr_enabled)
            eval_lj_attr_.declare_vars(def_ctx);

        auto vel_afm_enabled = params["velocity AFM"]["enabled"].as<bool>();
        eval_velocity_afm_forces eval_vel_afm_;
        if (vel_afm_enabled)
            eval_vel_afm_.declare_vars(def_ctx);

        auto force_afm_enabled = params["force AFM"]["enabled"].as<bool>();
        eval_force_afm_forces eval_force_afm_;
        if (force_afm_enabled)
            eval_force_afm_.declare_vars(def_ctx);

        auto create_checkpoint_enabled = params["checkpoints"]["enabled"].as<bool>();
        create_checkpoint create_checkpoint_;
        if (create_checkpoint_enabled)
            create_checkpoint_.declare_vars(def_ctx);

        auto& t = def_ctx.var<real>("t");
        auto &total_time = def_ctx.persistent<real>("total_time",
            params["general"]["total time"].as<quantity>());
        auto& invalid = def_ctx.var<bool>("invalid");

#pragma omp barrier

#pragma omp single
        {
            reset_vf_();
            nl_verify_();

            if (invalid) {
                legacy_update_();

                if (pauli_enabled)
                    update_pauli_();
                if (go_enabled)
                    update_go_();
                if (ss_enabled)
                    update_ss_();
                if (const_es_enabled)
                    update_const_es_();
                if (qa_enabled)
                    update_qa_();
                if (pid_enabled)
                    update_pid_();
            }
        };

#pragma omp barrier

        while (t < total_time) {
            reset_thread_vf_();

            if (chir_enabled)
                eval_chir_.omp_async();
            if (tethers_enabled)
                tethers_.omp_async();
            if (nat_ang_enabled)
                nat_ang_.omp_async();
            if (nat_comp_dih_enabled)
                nat_comp_dih_.omp_async();
            if (nat_simp_dih_enabled)
                nat_simp_dih_.omp_async();
            if (pauli_enabled)
                eval_pauli_.omp_async();
            if (go_enabled)
                eval_go_.omp_async();
            if (ss_enabled)
                eval_ss_.omp_async();
            if (const_es_enabled)
                eval_const_es_.omp_async();
            if (rel_es_enabled)
                eval_rel_es_.omp_async();
            if (qa_enabled) {
                eval_qa_.precompute_nh_t.omp_async();
                eval_qa_.sift_candidates_t.omp_async();
                eval_qa_.process_contacts_t.omp_async();
            }
            if (pid_enabled)
                eval_pid_.omp_async();
            if (solid_enabled)
                eval_solid_.omp_async();
            if (lj_attr_enabled)
                eval_lj_attr_.omp_async();
            if (vel_afm_enabled)
                eval_vel_afm_.omp_async();
            if (force_afm_enabled)
                eval_force_afm_.omp_async();

            reduce_vf_omp_();

#pragma omp barrier

#pragma omp master
            {
                if (export_pdb_enabled)
                    export_pdb_();
                if (show_pbar_enabled)
                    show_pbar_();
                if (report_stats_enabled)
                    report_stats_();
                if (report_structure_enabled)
                    report_structure_();
                if (create_checkpoint_enabled)
                    create_checkpoint_();

                if (lang_pc_enabled)
                    lang_pc_();

                reset_vf_();
                nl_verify_();
                if (qa_enabled) {
                    eval_qa_.process_candidates_t();
                    eval_qa_.sift_candidates_t.omp_prep();
                }

                if (invalid) {
                    legacy_update_();

                    if (pauli_enabled)
                        update_pauli_();
                    if (go_enabled)
                        update_go_();
                    if (ss_enabled)
                        update_ss_();
                    if (const_es_enabled)
                        update_const_es_();
                    if (rel_es_enabled)
                        update_rel_es_();
                    if (qa_enabled)
                        update_qa_();
                    if (pid_enabled)
                        update_pid_();
                }
            }

#pragma omp barrier
        }
    }

    return EXIT_SUCCESS;
}
