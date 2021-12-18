#include "forces/qa/qa.h"

namespace xmd::qa {
    void eval_qa_forces::init_from_vm(vm &vm_inst) {
        vm_inst.find_or_emplace<free_pair_set>("qa_free_pairs");
        vm_inst.find_or_emplace<candidate_list>("qa_candidates");
        vm_inst.find_or_emplace<contact_set>("qa_contacts");

        vm_inst.find_or<sync_data_vector>("sync", [&]() -> auto& {
            auto num_particles = vm_inst.find<int>("num_particles");
            auto& sync_vec_ = vm_inst.emplace<sync_data_vector>("sync",
                num_particles);

            auto& aa_data_ = vm_inst.find<amino_acid_data>(
                "amino_acid_data");
            auto& atype = vm_inst.find<vector<amino_acid>>("atype");

            for (int idx = 0; idx < num_particles; ++idx) {
                auto const& lim = aa_data_[atype[idx]].limits;
                sync_vec_.back[idx] = lim.back;
                sync_vec_.side_all[idx] = lim.side_all;
                sync_vec_.side_polar[idx] = lim.side_polar;
                sync_vec_.side_hydrophobic[idx] = lim.side_hydrophobic;
            }

            return sync_vec_;
        });

        precompute_nh_t.init_from_vm(vm_inst);
        sift_candidates_t.init_from_vm(vm_inst);
        process_candidates_t.init_from_vm(vm_inst);
        process_contacts_t.init_from_vm(vm_inst);
    }

    void eval_qa_forces::operator()() const {
//#pragma omp task default(none)
        {
            precompute_nh_t();
//#pragma omp taskwait
            sift_candidates_t();
//#pragma omp taskwait
            process_candidates_t();
//#pragma omp taskwait
            process_contacts_t();
        }
    }

    void update_qa::init_from_vm(vm &vm_inst) {
        update_free_pairs_.init_from_vm(vm_inst);
        eval = &vm_inst.find<eval_qa_forces>("eval_qa");
    }

    void update_qa::operator()() const {
        update_free_pairs_();
    }
}