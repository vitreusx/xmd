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

    int seed = 2137;
    std::default_random_engine eng(seed);
    model.morph_into_saw(eng, 3.8 * angstrom, 1e-3 * atom / pow(angstrom, 3.0),
        false);

    model_loader(&model).init_from_vm(def_vm);
    auto &t = def_vm.find_or_emplace<real>("t", (real) 0.0);
    def_vm.find_or_emplace<real>("V", (real) 0.0);
    def_vm.find_or_emplace<xmd::rand_gen>("gen", seed);

    auto &lang_pc_ = def_vm.find_or_emplace<lang_pc_step>("lang_pc");
    auto &reset_vf_ = def_vm.find_or_emplace<reset_vf>("reset_vf");
    auto &tethers_ = def_vm.find_or_emplace<eval_tether_forces>("eval_tether");
    auto &nat_ang_ = def_vm.find_or_emplace<eval_native_angle_forces>(
        "eval_nat_ang");
    auto &nat_comp_dih_ = def_vm.find_or_emplace<eval_cnd_forces>("eval_cnd");

    auto &divide_into_cells_ = def_vm.find_or_emplace<nl::divide_into_cells>(
        "div_into_cells");
    auto &nl_verify_ = def_vm.find_or_emplace<nl::verify>("nl_verify");
    auto &invalid = def_vm.find<bool>("invalid");

    auto &eval_pauli_ = def_vm.find_or_emplace<eval_pauli_exclusion_forces>(
        "eval_pauli");
    auto &update_pauli_ = def_vm.find_or_emplace<update_pauli_pairs>(
        "update_pauli");

    auto &eval_go_ = def_vm.find_or_emplace<eval_go_forces>("eval_go");
    auto &update_go_ = def_vm.find_or_emplace<update_go_contacts>("update_go");

    auto &eval_ss_ = def_vm.find_or_emplace<eval_nat_ssbond_forces>("eval_ss");
    auto &update_ss_ = def_vm.find_or_emplace<update_nat_ssbonds>("update_ss");

    auto &eval_const_es_ = def_vm.find_or_emplace<eval_const_es_forces>(
        "eval_const_es");
    auto &update_const_es_ = def_vm.find_or_emplace<update_const_es>(
        "update_const_es");

    auto &eval_qa_ = def_vm.find_or_emplace<qa::eval_qa_forces>("qa");
    auto &update_qa_ = def_vm.find_or_emplace<qa::update_free_pairs>(
        "update_qa");

    auto &eval_solid_ = def_vm.find_or_emplace<eval_solid_wall_forces>("eval_solid");
    auto &eval_lj_attr_ = def_vm.find_or_emplace<eval_lj_attr_wall_forces>("eval_lj_attr");

    auto &eval_vel_afm_ = def_vm.find_or_emplace<eval_velocity_afm_forces>("eval_vel_afm");
    auto &eval_force_afm_ = def_vm.find_or_emplace<eval_force_afm_forces>("eval_force_afm");

    auto &export_pdb_ = def_vm.find_or_emplace<export_pdb>("export_pdb");
    auto &show_pbar_ = def_vm.emplace<show_progress_bar>("show_progress_bar");
    auto &report_stats_ = def_vm.emplace<report_stats>("report_stats");
    auto &report_structure_ = def_vm.emplace<report_structure>(
        "report_structure");

    auto &total_time = def_vm.find_or_emplace<real>("total_time",
        params["general"]["total time"].as<quantity>());

    reset_vf_();
    nl_verify_();

    if (invalid) {
        divide_into_cells_();

        update_pauli_();
        update_go_();
        update_ss_();
        update_const_es_();
        update_qa_();
    }

#pragma omp parallel
    {
        while (t < total_time) {
            tethers_.omp_async();
            nat_ang_.omp_async();
            nat_comp_dih_.omp_async();
            eval_pauli_.omp_async();
            eval_go_.omp_async();
            eval_ss_.omp_async();
            eval_const_es_.omp_async();
            eval_qa_.precompute_nh_t.omp_async();
            eval_qa_.sift_candidates_t.omp_async();
            eval_qa_.process_contacts_t.omp_async();
            eval_solid_.omp_async();
            eval_lj_attr_.omp_async();
            eval_vel_afm_.omp_async();
            eval_force_afm_.omp_async();

#pragma omp barrier

#pragma omp master
            {
                export_pdb_();
                show_pbar_();
                report_stats_();
                report_structure_();

                lang_pc_();

                reset_vf_();
                nl_verify_();
                eval_qa_.process_candidates_t();
                eval_qa_.sift_candidates_t.omp_prep();

                if (invalid) {
                    divide_into_cells_();

                    update_pauli_();
                    update_go_();
                    update_ss_();
                    update_const_es_();
                    update_qa_();
                }
            }

#pragma omp barrier
        }
    };

    return EXIT_SUCCESS;
}
