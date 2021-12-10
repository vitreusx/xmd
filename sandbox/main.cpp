#include <fstream>
#include <iostream>
#include <xmd/files/pdb.h>
#include <xmd/model/model.h>
#include <xmd/utils/units.h>
#include <xmd/vm/vm.h>
#include <xmd/model/loader.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/lang_pc.h>
#include <xmd/forces/tether.h>
#include <xmd/forces/angle/native.h>
#include <xmd/forces/dihedral/complex_native.h>
#include <xmd/io/show_progress_bar.h>
#include <xmd/io/export_pdb.h>
#include <xmd/stats/total_energy.h>
#include <xmd/nl/verify.h>
#include <xmd/nl/divide_into_cells.h>
#include <xmd/files/seq_file.h>
using namespace xmd;

int main() {
    auto seqfile = seq_file("data/examples/glut/glut.yml");
    auto model = seqfile.to_model();

    int seed = 2137;
    std::default_random_engine eng(seed);
    model.morph_into_saw(eng, 3.8*angstrom, 1e-3*atom/pow(angstrom, 3.0),
        false);

    vm def_vm;
    model_loader(&model).init_from_vm(def_vm);
    def_vm.find_or_emplace<real>("V", (real)0.0);
    def_vm.find_or_emplace<xorshift64>("gen", seed);

    auto& lang_pc_ = def_vm.emplace<lang_pc_step>("lang_pc");
    lang_pc_.dt = 5e-3*tau;
    lang_pc_.gamma_factor = 2.0/tau;
    lang_pc_.temperature = 0.38*eps/kB;

    auto& reset_vf_ = def_vm.emplace<reset_vf>("reset_vf");

    eval_tether_forces tethers;
    tethers.H1 = 100.0*eps/pow(angstrom, 2.0);
    tethers.H2 = 0.0;
    tethers.def_length = 3.8*angstrom;
    auto& tethers_ = def_vm.emplace<eval_tether_forces>("eval_tether", tethers);

    auto& nat_ang_ = def_vm.emplace<eval_native_angle_forces>("eval_nat_ang");
    nat_ang_.k = 30.0*eps/pow(rad, 2.0);

    auto& nat_comp_dih_ = def_vm.emplace<eval_cnd_forces>("eval_cnd");
    nat_comp_dih_.CDA = nat_comp_dih_.CDB = 3.33*eps/pow(rad, 2.0);

    auto& export_pdb_ = def_vm.emplace<export_pdb>("export_pdb");
    export_pdb_.out_file_path = "output.pdb";
    auto export_pdb_period = 100.0*tau;
    auto last_export_pdb_t = std::numeric_limits<real>::lowest();

    auto& comp_total_energy_ = def_vm.emplace<compute_total_energy>("comp_tot_E");

    auto& show_pbar_ = def_vm.emplace<show_progress_bar>("show_progress_bar");
    auto total_t = 20e3*tau;
    show_pbar_.width = 50;
    show_pbar_.total_time = total_t;
    show_pbar_.start_wall_time = std::chrono::high_resolution_clock::now();
    auto show_pbar_period = 25.0*tau;
    auto last_show_pbar_t = std::numeric_limits<real>::lowest();

    auto& divide_into_cells_ = def_vm.emplace<nl::divide_into_cells>(
        "divide_into_cells");
    auto& verify_ = def_vm.emplace<nl::verify>("nl_verify");

    *divide_into_cells_.cutoff = 10.0*angstrom;
    *divide_into_cells_.pad = 5.0*angstrom;

    auto& t = def_vm.find<real>("t");
    while (t < total_t) {
        reset_vf_();

        verify_();
        if (*verify_.invalid)
            divide_into_cells_();

        tethers_();
        nat_ang_();
        nat_comp_dih_();

        if (t - last_export_pdb_t >= export_pdb_period) {
            export_pdb_();
            last_export_pdb_t = t;
        }

        if (t - last_show_pbar_t >= show_pbar_period) {
            comp_total_energy_();
            show_pbar_();
            last_show_pbar_t = t;
        }

        lang_pc_();
    }

    return EXIT_SUCCESS;
}
