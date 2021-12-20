#include <fstream>
#include <iostream>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <xmd/model/model.h>
#include <xmd/utils/units.h>
#include <xmd/vm/vm.h>
#include <xmd/model/loader.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/lang_pc.h>
#include <xmd/forces/all.h>
#include <xmd/io/show_progress_bar.h>
#include <xmd/io/export_pdb.h>
#include <xmd/params/param_file.h>
#include <xmd/nl/verify.h>
#include <xmd/nl/divide_into_cells.h>
#include <xmd/io/report_stats.h>
#include <xmd/io/report_structure.h>
#include <omp.h>

using namespace xmd;

int main() {
    vm def_vm;
    auto &params = def_vm.emplace<param_file>("params",
        "data/examples/defaults.yml");
    def_vm.find_or_emplace<amino_acid_data>("amino_acid_data",
        params["amino acid data"].as<amino_acid_data>());
    def_vm.find_or_emplace<lj_variants>("lj_variants");

    auto seqfile = seq_file("data/examples/glut/glut.yml");
    auto model = seqfile.to_model();

    int seed = def_vm.find_or_emplace<int>("seed",
        params["general"]["seed"].as<int>());
    std::default_random_engine eng(seed);
    model.morph_into_saw(eng, 3.8 * angstrom, 1e-3 * atom / pow(angstrom, 3.0),
        false);

    model_loader(&model).init_from_vm(def_vm);
    auto &t = def_vm.find_or_emplace<real>("t", (real) 0.0);
    def_vm.find_or_emplace<real>("V", (real) 0.0);
    def_vm.find_or_emplace<xmd::rand_gen>("gen", seed);

    auto &lang_pc_enabled = def_vm.find_or_emplace<bool>("lang_pc_enabled",
        params["langevin"]["enabled"].as<bool>());
    auto &lang_pc_ = def_vm.find_or_emplace<lang_pc_step>("lang_pc");

    auto &reset_vf_ = def_vm.find_or_emplace<reset_vf>("reset_vf");

    auto &chir_enabled = def_vm.find_or_emplace<bool>("chir_enabled",
        params["chirality"]["enabled"].as<bool>());
    auto &eval_chir_ = def_vm.find_or_emplace<eval_chiral_forces>("eval_chir");

    auto &tethers_enabled = def_vm.find_or_emplace<bool>("tethers_enabled",
        params["tether forces"]["enabled"].as<bool>());
    auto &tethers_ = def_vm.find_or_emplace<eval_tether_forces>("eval_tether");

    auto &nat_ang_enabled = def_vm.find_or_emplace<bool>("nat_ang_enabled",
        params["native angles"]["enabled"].as<bool>());
    auto &nat_ang_ = def_vm.find_or_emplace<eval_native_angle_forces>(
        "eval_nat_ang");

    auto &nat_comp_dih_enabled = def_vm.find_or_emplace<bool>("nat_comp_dih_enabled",
        params["complex native dihedrals"]["enabled"].as<bool>());
    auto &nat_comp_dih_ = def_vm.find_or_emplace<eval_cnd_forces>("eval_cnd");

    auto &nat_simp_dih_enabled = def_vm.find_or_emplace<bool>("nat_simp_dih_enabled",
        params["simple native dihedrals"]["enabled"].as<bool>());
    auto &nat_simp_dih_ = def_vm.find_or_emplace<eval_snd_forces>("eval_snd");

    auto &divide_into_cells_ = def_vm.find_or_emplace<nl::divide_into_cells>(
        "div_into_cells");
    auto &nl_verify_ = def_vm.find_or_emplace<nl::verify>("nl_verify");
    auto &invalid = def_vm.find<bool>("invalid");

    auto &pauli_enabled = def_vm.find_or_emplace<bool>("pauli_enabled",
        params["Pauli exclusion"]["enabled"].as<bool>());
    auto &eval_pauli_ = def_vm.find_or_emplace<eval_pauli_exclusion_forces>(
        "eval_pauli");
    auto &update_pauli_ = def_vm.find_or_emplace<update_pauli_pairs>(
        "update_pauli");

    auto &go_enabled = def_vm.find_or_emplace<bool>("go_enabled",
        params["native contacts"]["enabled"].as<bool>());
    auto &eval_go_ = def_vm.find_or_emplace<eval_go_forces>("eval_go");
    auto &update_go_ = def_vm.find_or_emplace<update_go_contacts>("update_go");

    auto &ss_enabled = def_vm.find_or_emplace<bool>("ss_enabled",
        params["native ssbonds"]["enabled"].as<bool>());
    auto &eval_ss_ = def_vm.find_or_emplace<eval_nat_ssbond_forces>("eval_ss");
    auto &update_ss_ = def_vm.find_or_emplace<update_nat_ssbonds>("update_ss");

    auto &const_es_enabled = def_vm.find_or_emplace<bool>("const_es_enabled",
        params["const ES"]["enabled"].as<bool>());
    auto &eval_const_es_ = def_vm.find_or_emplace<eval_const_es_forces>(
        "eval_const_es");
    auto &update_const_es_ = def_vm.find_or_emplace<update_const_es>(
        "update_const_es");

    auto &rel_es_enabled = def_vm.find_or_emplace<bool>("rel_es_enabled",
        params["relative ES"]["enabled"].as<bool>());
    auto &eval_rel_es_ = def_vm.find_or_emplace<eval_relative_es_forces>(
        "eval_rel_es");
    auto &update_rel_es_ = def_vm.find_or_emplace<update_relative_es>(
        "update_rel_es");

    auto &qa_enabled = def_vm.find_or_emplace<bool>("qa_enabled",
        params["quasi-adiabatic"]["enabled"].as<bool>());
    auto &eval_qa_ = def_vm.find_or_emplace<qa::eval_qa_forces>("eval_qa");
    auto &update_qa_ = def_vm.find_or_emplace<qa::update_free_pairs>(
        "update_qa");

    auto &pid_enabled = def_vm.find_or_emplace<bool>("pid_enabled",
        params["pseudo-improper dihedral"]["enabled"].as<bool>());
    auto &eval_pid_ = def_vm.find_or_emplace<pid::eval_pid_forces>("eval_pid");
    auto &update_pid_ = def_vm.find_or_emplace<pid::update_pid_bundles>("update_pid");

    auto &solid_enabled = def_vm.find_or_emplace<bool>("solid_enabled",
        params["solid walls"]["enabled"].as<bool>());
    auto &eval_solid_ = def_vm.find_or_emplace<eval_solid_wall_forces>("eval_solid");

    auto &lj_attr_enabled = def_vm.find_or_emplace<bool>("lj_attr_enabled",
        params["LJ attractive walls"]["enabled"].as<bool>());
    auto &eval_lj_attr_ = def_vm.find_or_emplace<eval_lj_attr_wall_forces>("eval_lj_attr");

    auto &vel_afm_enabled = def_vm.find_or_emplace<bool>("vel_afm_enabled",
        params["velocity AFM"]["enabled"].as<bool>());
    auto &eval_vel_afm_ = def_vm.find_or_emplace<eval_velocity_afm_forces>("eval_vel_afm");

    auto &force_afm_enabled = def_vm.find_or_emplace<bool>("force_afm_enabled",
        params["force AFM"]["enabled"].as<bool>());
    auto &eval_force_afm_ = def_vm.find_or_emplace<eval_force_afm_forces>("eval_force_afm");

    auto &export_pdb_enabled = def_vm.find_or_emplace<bool>("export_pdb_enabled",
        params["export pdb"]["enabled"].as<bool>());
    auto &export_pdb_ = def_vm.find_or_emplace<export_pdb>("export_pdb");

    auto &show_pbar_enabled = def_vm.find_or_emplace<bool>("show_pbar_enabled",
        params["progress bar"]["enabled"].as<bool>());
    auto &show_pbar_ = def_vm.emplace<show_progress_bar>("show_progress_bar");

    auto &report_stats_enabled = def_vm.find_or_emplace<bool>("report_stats_enabled",
        params["report stats"]["enabled"].as<bool>());
    auto &report_stats_ = def_vm.emplace<report_stats>("report_stats");

    auto &report_structure_enabled = def_vm.find_or_emplace<bool>("report_structure_enabled",
        params["report structure"]["enabled"].as<bool>());
    auto &report_structure_ = def_vm.emplace<report_structure>(
        "report_structure");

    auto &total_time = def_vm.find_or_emplace<real>("total_time",
        params["general"]["total time"].as<quantity>());

    reset_vf_();
    nl_verify_();

    if (invalid) {
        divide_into_cells_();

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

#pragma omp parallel
    {
        while (t < total_time) {
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
            if (solid_enabled)
                eval_solid_.omp_async();
            if (lj_attr_enabled)
                eval_lj_attr_.omp_async();
            if (vel_afm_enabled)
                eval_vel_afm_.omp_async();
            if (force_afm_enabled)
                eval_force_afm_.omp_async();

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

                if (lang_pc_enabled)
                    lang_pc_();

                reset_vf_();
                nl_verify_();
                if (qa_enabled) {
                    eval_qa_.process_candidates_t();
                    eval_qa_.sift_candidates_t.omp_prep();
                }

                if (invalid) {
                    divide_into_cells_();

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
            }

#pragma omp barrier
        }
    };

    return EXIT_SUCCESS;
}
