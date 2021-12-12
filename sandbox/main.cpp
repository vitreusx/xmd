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
#include <xmd/stats/total_energy.h>
#include <xmd/params/param_file.h>
#include <xmd/nl/verify.h>
#include <xmd/nl/divide_into_cells.h>
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

    auto& eval_qa_ = def_vm.find_or_emplace<qa::eval_qa_forces>("qa");
    auto& update_qa_ = def_vm.find_or_emplace<qa::update_free_pairs>("update_qa");

    auto& export_pdb_ = def_vm.find_or_emplace<export_pdb>("export_pdb");
    auto& show_pbar_ = def_vm.emplace<show_progress_bar>("show_progress_bar");

    auto& total_time = def_vm.find_or_emplace<real>("total_time",
        params["general"]["total time"].as<quantity>());

    while (t < total_time) {
        reset_vf_();

        nl_verify_();
        if (invalid) {
            divide_into_cells_();
            update_pauli_();
            eval_pauli_.pairs = update_pauli_.pairs->to_span();
            update_go_();
            eval_go_.contacts = update_go_.all_contacts->to_span();
            update_ss_();
            eval_ss_.ssbonds = update_ss_.ssbonds->to_span();
            update_es_();
            eval_const_es_.es_pairs = update_es_.pairs->to_span();
            update_qa_();
            eval_qa_.sift_candidates_t.free_pairs = update_qa_.pairs;
            eval_qa_.process_candidates_t.free_pairs = update_qa_.pairs;
        }

        tethers_();
        nat_ang_();
        nat_comp_dih_();
        eval_pauli_();
        eval_go_();
        eval_ss_();
        eval_const_es_();
        eval_qa_();

        export_pdb_();
        show_pbar_();

        lang_pc_();
    }

    return EXIT_SUCCESS;
}
