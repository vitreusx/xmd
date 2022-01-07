#include <xmd/utils/units.h>
#include <xmd/ctx/context.h>
#include <xmd/model/loader.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/setup_vf_omp.h>
#include <xmd/dynamics/reduce_vf_omp.h>
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
    std::string param_path = "parameters.yml";

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

    auto& seed = def_ctx.ephemeral<int>("seed",
        params["general"]["seed"].as<int>());
    def_ctx.ephemeral<xmd::rand_gen>("gen", seed);

    model_loader().declare_vars(def_ctx);

    auto &t = def_ctx.persistent<real>("t", (real) 0.0);
    def_ctx.persistent<real>("V", (real) 0.0);
    def_ctx.ephemeral<amino_acid_data>("amino_acid_data",
        params["amino acid data"].as<amino_acid_data>());
    def_ctx.ephemeral<lj_variants>("lj_variants");

    auto &lang_pc_enabled = def_ctx.ephemeral<bool>("lang_pc_enabled",
        params["langevin"]["enabled"].as<bool>());
    auto &lang_pc_ = def_ctx.ephemeral<lang_pc_step>("lang_pc");

    auto &reset_vf_ = def_ctx.ephemeral<reset_vf>("reset_vf");

    auto &chir_enabled = def_ctx.ephemeral<bool>("chir_enabled",
        params["chirality"]["enabled"].as<bool>());
    auto &eval_chir_ = def_ctx.ephemeral<eval_chiral_forces>("eval_chir");

    auto &tethers_enabled =def_ctx.ephemeral<bool>("tethers_enabled",
        params["tether forces"]["enabled"].as<bool>());
    auto &tethers_ = def_ctx.ephemeral<eval_tether_forces>("eval_tether");

    auto &nat_ang_enabled = def_ctx.ephemeral<bool>("nat_ang_enabled",
        params["native angles"]["enabled"].as<bool>());
    auto &nat_ang_ = def_ctx.ephemeral<eval_native_angle_forces>(
        "eval_nat_ang");

    auto &nat_comp_dih_enabled = def_ctx.ephemeral<bool>("nat_comp_dih_enabled",
        params["complex native dihedrals"]["enabled"].as<bool>());
    auto &nat_comp_dih_ = def_ctx.ephemeral<eval_cnd_forces>("eval_cnd");

    auto &nat_simp_dih_enabled = def_ctx.ephemeral<bool>("nat_simp_dih_enabled",
        params["simple native dihedrals"]["enabled"].as<bool>());
    auto &nat_simp_dih_ = def_ctx.ephemeral<eval_snd_forces>("eval_snd");

    auto &legacy_update_ = def_ctx.ephemeral<nl::legacy_update>("legacy_update");
    auto &nl_verify_ = def_ctx.ephemeral<nl::verify>("nl_verify");
    auto &invalid = def_ctx.ephemeral<bool>("invalid", true);

    auto &pauli_enabled = def_ctx.ephemeral<bool>("pauli_enabled",
        params["Pauli exclusion"]["enabled"].as<bool>());
    auto &eval_pauli_ = def_ctx.ephemeral<eval_pauli_exclusion_forces>(
        "eval_pauli");
    auto &update_pauli_ = def_ctx.ephemeral<update_pauli_pairs>(
        "update_pauli");

    auto &go_enabled = def_ctx.ephemeral<bool>("go_enabled",
        params["native contacts"]["enabled"].as<bool>());
    auto &eval_go_ = def_ctx.ephemeral<eval_go_forces>("eval_go");
    auto &update_go_ = def_ctx.ephemeral<update_go_contacts>("update_go");

    auto &ss_enabled = def_ctx.ephemeral<bool>("ss_enabled",
        params["native ssbonds"]["enabled"].as<bool>());
    auto &eval_ss_ = def_ctx.ephemeral<eval_nat_ssbond_forces>("eval_ss");
    auto &update_ss_ = def_ctx.ephemeral<update_nat_ssbonds>("update_ss");

    auto &const_es_enabled = def_ctx.ephemeral<bool>("const_es_enabled",
        params["const ES"]["enabled"].as<bool>());
    auto &eval_const_es_ = def_ctx.ephemeral<eval_const_es_forces>(
        "eval_const_es");
    auto &update_const_es_ = def_ctx.ephemeral<update_const_es>(
        "update_const_es");

    auto &rel_es_enabled = def_ctx.ephemeral<bool>("rel_es_enabled",
        params["relative ES"]["enabled"].as<bool>());
    auto &eval_rel_es_ = def_ctx.ephemeral<eval_relative_es_forces>(
        "eval_rel_es");
    auto &update_rel_es_ = def_ctx.ephemeral<update_relative_es>(
        "update_rel_es");

    auto &qa_enabled = def_ctx.ephemeral<bool>("qa_enabled",
        params["quasi-adiabatic"]["enabled"].as<bool>());
    auto &eval_qa_ = def_ctx.ephemeral<qa::eval_qa_forces>("eval_qa");
    auto &update_qa_ = def_ctx.ephemeral<qa::update_free_pairs>(
        "update_qa");

    auto &pid_enabled = def_ctx.ephemeral<bool>("pid_enabled",
        params["pseudo-improper dihedral"]["enabled"].as<bool>());
    auto &eval_pid_ = def_ctx.ephemeral<pid::eval_pid_forces>("eval_pid");
    auto &update_pid_ = def_ctx.ephemeral<pid::update_pid_bundles>("update_pid");

    auto &solid_enabled = def_ctx.ephemeral<bool>("solid_enabled",
        params["solid walls"]["enabled"].as<bool>());
    auto &eval_solid_ = def_ctx.ephemeral<eval_solid_wall_forces>("eval_solid");

    auto &lj_attr_enabled = def_ctx.ephemeral<bool>("lj_attr_enabled",
        params["LJ attractive walls"]["enabled"].as<bool>());
    auto &eval_lj_attr_ = def_ctx.ephemeral<eval_lj_attr_wall_forces>("eval_lj_attr");

    auto &vel_afm_enabled = def_ctx.ephemeral<bool>("vel_afm_enabled",
        params["velocity AFM"]["enabled"].as<bool>());
    auto &eval_vel_afm_ = def_ctx.ephemeral<eval_velocity_afm_forces>("eval_vel_afm");

    auto &force_afm_enabled = def_ctx.ephemeral<bool>("force_afm_enabled",
        params["force AFM"]["enabled"].as<bool>());
    auto &eval_force_afm_ = def_ctx.ephemeral<eval_force_afm_forces>("eval_force_afm");

    auto &export_pdb_enabled = def_ctx.ephemeral<bool>("export_pdb_enabled",
        params["export pdb"]["enabled"].as<bool>());
    auto &export_pdb_ = def_ctx.ephemeral<export_pdb>("export_pdb");

    auto &show_pbar_enabled = def_ctx.ephemeral<bool>("show_pbar_enabled",
        params["progress bar"]["enabled"].as<bool>());
    auto &show_pbar_ = def_ctx.ephemeral<show_progress_bar>("show_progress_bar");

    auto &report_stats_enabled = def_ctx.ephemeral<bool>("report_stats_enabled",
        params["report stats"]["enabled"].as<bool>());
    auto &report_stats_ = def_ctx.ephemeral<report_stats>("report_stats");

    auto &report_structure_enabled = def_ctx.ephemeral<bool>("report_structure_enabled",
        params["report structure"]["enabled"].as<bool>());
    auto &report_structure_ = def_ctx.ephemeral<report_structure>(
        "report_structure");

    auto& create_checkpoint_enabled = def_ctx.ephemeral<bool>("create_checkpoint_enabled",
        params["checkpoints"]["enabled"].as<bool>());
    auto& create_checkpoint_ = def_ctx.ephemeral<create_checkpoint>(
        "create_checkpoint");

    auto &total_time = def_ctx.ephemeral<real>("total_time",
        params["general"]["total time"].as<quantity>());

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

#pragma omp parallel default(shared) firstprivate(eval_chir_,tethers_,nat_ang_,\
nat_comp_dih_,eval_pauli_,eval_go_,eval_ss_,eval_const_es_,eval_rel_es_,\
eval_qa_,eval_solid_,eval_lj_attr_,eval_vel_afm_,eval_force_afm_)
    {
        auto thread_ctx = def_ctx;
        auto& setup_vf_omp_ = thread_ctx.ephemeral<setup_vf_omp>("setup_vf_omp");
        auto& reduce_vf_omp_ = thread_ctx.ephemeral<reduce_vf_omp>("reduce_vf_omp");

        eval_chir_.declare_vars(thread_ctx);
        tethers_.declare_vars(thread_ctx);
        nat_ang_.declare_vars(thread_ctx);
        nat_comp_dih_.declare_vars(thread_ctx);
        nat_simp_dih_.declare_vars(thread_ctx);
        eval_pauli_.declare_vars(thread_ctx);
        eval_go_.declare_vars(thread_ctx);
        eval_ss_.declare_vars(thread_ctx);
        eval_const_es_.declare_vars(thread_ctx);
        eval_rel_es_.declare_vars(thread_ctx);
        eval_qa_.declare_vars(thread_ctx);
        eval_solid_.declare_vars(thread_ctx);
        eval_lj_attr_.declare_vars(thread_ctx);
        eval_vel_afm_.declare_vars(thread_ctx);
        eval_force_afm_.declare_vars(thread_ctx);

        while (t < total_time) {
            reset_vf_.omp_async();
            setup_vf_omp_();

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
    };

    return EXIT_SUCCESS;
}
