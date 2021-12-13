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

using namespace xmd;

int main() {
    vm def_vm;
    auto& params = def_vm.emplace<param_file>("params",
        "data/examples/defaults.yml");
    def_vm.find_or_emplace<amino_acid_data>("amino_acid_data",
        params["amino acid data"].as<amino_acid_data>());
    def_vm.find_or_emplace<lj_variants>("lj_variants");

    auto seqfile = seq_file("data/examples/glut/glut.yml");
    auto model = seqfile.to_model();

    int seed = 2137;
    std::default_random_engine eng(seed);
    model.morph_into_saw(eng, 3.8*angstrom, 1e-3*atom/pow(angstrom, 3.0),
        false);

    model_loader(&model).init_from_vm(def_vm);
    auto& t = def_vm.find_or_emplace<real>("t", (real)0.0);
    def_vm.find_or_emplace<real>("V", (real)0.0);
    def_vm.find_or_emplace<xmd::rand_gen>("gen", seed);

    auto& lang_pc_ = def_vm.find_or_emplace<lang_pc_step>("lang_pc");
    auto& reset_vf_ = def_vm.find_or_emplace<reset_vf>("reset_vf");
    auto& tethers_ = def_vm.find_or_emplace<eval_tether_forces>("eval_tether");
    auto& nat_ang_ = def_vm.find_or_emplace<eval_native_angle_forces>("eval_nat_ang");
    auto& nat_comp_dih_ = def_vm.find_or_emplace<eval_cnd_forces>("eval_cnd");

    auto& divide_into_cells_ = def_vm.find_or_emplace<nl::divide_into_cells>("div_into_cells");
    auto& nl_verify_ = def_vm.find_or_emplace<nl::verify>("nl_verify");
    auto& invalid = def_vm.find<bool>("invalid");

    auto& eval_pauli_ = def_vm.find_or_emplace<eval_pauli_exclusion_forces>("eval_pauli");
    auto& update_pauli_ = def_vm.find_or_emplace<update_pauli_pairs>("update_pauli");

    auto& eval_go_ = def_vm.find_or_emplace<eval_go_forces>("eval_go");
    auto& update_go_ = def_vm.find_or_emplace<update_go_contacts>("update_go");

    auto& eval_ss_ = def_vm.find_or_emplace<eval_nat_ssbond_forces>("eval_ss");
    auto& update_ss_ = def_vm.find_or_emplace<update_nat_ssbonds>("update_ss");

    auto& eval_const_es_ = def_vm.find_or_emplace<eval_const_es_forces>("eval_const_es");
    auto& update_es_ = def_vm.find_or_emplace<update_es_pairs>("update_es");

    auto& eval_qa_tf_ = def_vm.find_or_emplace<qa::eval_qa_forces_tf>("qa");
    auto& update_qa_ = def_vm.find_or_emplace<qa::update_free_pairs>("update_qa");

    auto& export_pdb_ = def_vm.find_or_emplace<export_pdb>("export_pdb");
    auto& show_pbar_ = def_vm.emplace<show_progress_bar>("show_progress_bar");
    auto& report_stats_ = def_vm.emplace<report_stats>("report_stats");
    auto& report_structure_ = def_vm.emplace<report_structure>("report_structure");

    auto& total_time = def_vm.find_or_emplace<real>("total_time",
        params["general"]["total time"].as<quantity>());

    auto exec = tf::Executor();
    auto taskflow = tf::Taskflow();

    auto init_tf = taskflow.emplace([](){})
        .name("program start");

    auto exit_tf = taskflow.emplace([](){})
        .name("program exit");

    auto while_cond_tf = taskflow.emplace([&]() -> auto {
        return t < total_time;
    }).name("while (t < total_time)");

    auto loop_pass = tf::Taskflow();

    auto reset_vf_tf = loop_pass.emplace(reset_vf_)
        .name("reset_vf()");

    auto nl_verify_phase = tf::Taskflow();
    auto nl_verify_tf = nl_verify_phase.emplace(nl_verify_)
        .name("nl_verify()");
    auto invalid_check_tf = nl_verify_phase.emplace([&]() -> auto {
        return invalid;
    }).name("if (invalid)");
    auto if_valid_tf = nl_verify_phase.emplace([](){})
        .name("(after if)");

    auto if_invalid_pass = tf::Taskflow();
    auto divide_into_cells_tf = if_invalid_pass.emplace(divide_into_cells_)
        .name("divide_into_cells()");
    auto update_vars_tf = if_invalid_pass.emplace([&]() -> void {
        eval_pauli_.pairs = update_pauli_.pairs->to_span();
        eval_go_.contacts = update_go_.all_contacts->to_span();
        eval_ss_.ssbonds = update_ss_.ssbonds->to_span();
        eval_const_es_.es_pairs = update_es_.pairs->to_span();
        eval_qa_tf_.sift_candidates_tf_t.sift_candidates_.free_pairs =
            update_qa_.pairs;
        eval_qa_tf_.process_candidates_t.free_pairs = update_qa_.pairs;
    }).name("[update vars]");

    auto updates_pass = tf::Taskflow();
    auto update_pauli_tf = updates_pass.emplace(update_pauli_)
        .name("update_pauli()");
    auto update_go_tf = updates_pass.emplace(update_go_)
        .name("update_go()");
    auto update_ss_tf = updates_pass.emplace(update_ss_)
        .name("update_ss()");
    auto update_es_tf = updates_pass.emplace(update_es_)
        .name("update_es()");
    auto update_qa_tf = updates_pass.emplace(update_qa_)
        .name("update_qa()");
    auto updates_pass_tf = if_invalid_pass.composed_of(updates_pass)
        .name("[update hooks]");

    divide_into_cells_tf.precede(updates_pass_tf);
    updates_pass_tf.precede(update_vars_tf);
    auto if_invalid_tf = nl_verify_phase.composed_of(if_invalid_pass)
        .name("[when nl is invalid]");

    nl_verify_tf.precede(invalid_check_tf);
    invalid_check_tf.precede(if_valid_tf, if_invalid_tf);
    auto nl_verify_phase_tf = loop_pass.composed_of(nl_verify_phase)
        .name("[nl verify phase]");

    auto forces_pass = tf::Taskflow();
    auto tethers_tf = tethers_.tf_impl(forces_pass)
        .name("tethers()");
    auto nat_ang_tf = nat_ang_.tf_impl(forces_pass)
        .name("nat_ang()");
    auto nat_comp_dih_tf = nat_comp_dih_.tf_impl(forces_pass)
        .name("nat_comp_dih()");
    auto eval_pauli_tf = eval_pauli_.tf_impl(forces_pass)
        .name("eval_pauli()");
    auto eval_go_tf = eval_go_.tf_impl(forces_pass)
        .name("eval_go()");
    auto eval_ss_tf = eval_ss_.tf_impl(forces_pass)
        .name("eval_ss()");
    auto eval_const_es_tf = eval_const_es_.tf_impl(forces_pass)
        .name("eval_const_es()");
    auto eval_qa_tf = eval_qa_tf_.tf_impl(forces_pass)
        .name("eval_qa()");
    auto forces_tf = loop_pass.composed_of(forces_pass)
        .name("[eval forces]");

    auto report_tf = loop_pass.emplace([&]() -> void {
        export_pdb_();
        show_pbar_();
        report_stats_();
        report_structure_();
    }).name("[report stuff]");

    auto lang_pc_tf = loop_pass.emplace(lang_pc_)
        .name("lang_pc()");

    reset_vf_tf.precede(nl_verify_phase_tf);
    nl_verify_phase_tf.precede(forces_tf);
    forces_tf.precede(report_tf);
    report_tf.precede(lang_pc_tf);

    auto loop_tf = taskflow.composed_of(loop_pass)
        .name("loop body");
    auto loop_exit_tf = taskflow.emplace([](){ return 0; })
        .name("loop exit");

    init_tf.precede(while_cond_tf);
    while_cond_tf.precede(exit_tf, loop_tf);
    loop_tf.precede(loop_exit_tf);
    loop_exit_tf.precede(while_cond_tf);

    exec.run(taskflow).wait();
//    taskflow.dump(std::cout);
    return EXIT_SUCCESS;
}
