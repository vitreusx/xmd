#include "forces/qa/qa.h"

namespace xmd::qa {
    void eval_qa_forces::init_from_vm(vm &vm_inst) {
        precompute_nh_t.init_from_vm(vm_inst);

        auto& free_pairs = vm_inst.find_or_emplace<free_pair_set>("qa_free_pairs");
        auto& candidates = vm_inst.find_or_emplace<candidate_list>("qa_candidates");
        auto& contacts = vm_inst.find_or_emplace<contact_set>("qa_contacts");

        auto sync = vm_inst.find_or<sync_data_vector>("sync", [&]() -> auto& {
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
        }).to_array();
        auto& t = vm_inst.find<real>("t");
        auto& box = vm_inst.find<xmd::box<vec3r>>("box");
        auto r = vm_inst.find<vec3r_vector>("r").to_array();

        sift_candidates_t.init_from_vm(vm_inst);
        sift_candidates_t.candidates = &candidates;
        sift_candidates_t.atype = vm_inst.find<vector<amino_acid>>("atype").to_array();
        sift_candidates_t.box = &box;
        sift_candidates_t.r = r;
        sift_candidates_t.free_pairs = &free_pairs;
        sift_candidates_t.sync = sync;
        sift_candidates_t.n = precompute_nh_t.n;
        sift_candidates_t.h = precompute_nh_t.h;

        process_candidates_t.candidates = &candidates;
        process_candidates_t.contacts = &contacts;
        process_candidates_t.sync = sync;
        process_candidates_t.free_pairs = &free_pairs;
        process_candidates_t.t = &t;

        process_contacts_t.init_from_vm(vm_inst);
        process_contacts_t.sync = sync;
        process_contacts_t.contacts = &contacts;
        process_contacts_t.box = &box;
        process_contacts_t.V = &vm_inst.find<real>("V");
        process_contacts_t.F = vm_inst.find<vec3r_vector>("F").to_array();
        process_contacts_t.r = r;
        process_contacts_t.free_pairs = &free_pairs;
    }

    void eval_qa_forces::operator()() const {
        precompute_nh_t();
        sift_candidates_t();
        process_candidates_t();
        process_contacts_t();
    }

    void eval_qa_forces_tf::init_from_vm(vm &vm_inst) {
        precompute_nh_t.init_from_vm(vm_inst);

        auto& free_pairs = vm_inst.find_or_emplace<free_pair_set>("qa_free_pairs");
        auto& candidates = vm_inst.find_or_emplace<candidate_list>("qa_candidates");
        auto& contacts = vm_inst.find_or_emplace<contact_set>("qa_contacts");

        auto sync = vm_inst.find_or<sync_data_vector>("sync", [&]() -> auto& {
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
        }).to_array();
        auto& t = vm_inst.find<real>("t");
        auto& box = vm_inst.find<xmd::box<vec3r>>("box");
        auto r = vm_inst.find<vec3r_vector>("r").to_array();

        auto& sift_candidates_t = sift_candidates_tf_t.sift_candidates_;
        sift_candidates_t.init_from_vm(vm_inst);
        sift_candidates_t.candidates = &candidates;
        sift_candidates_t.atype = vm_inst.find<vector<amino_acid>>("atype").to_array();
        sift_candidates_t.box = &box;
        sift_candidates_t.r = r;
        sift_candidates_t.free_pairs = &free_pairs;
        sift_candidates_t.sync = sync;
        sift_candidates_t.n = precompute_nh_t.n;
        sift_candidates_t.h = precompute_nh_t.h;
        sift_candidates_tf_t.init_from_vm(vm_inst);

        process_candidates_t.candidates = &candidates;
        process_candidates_t.contacts = &contacts;
        process_candidates_t.sync = sync;
        process_candidates_t.free_pairs = &free_pairs;
        process_candidates_t.t = &t;

        process_contacts_t.init_from_vm(vm_inst);
        process_contacts_t.sync = sync;
        process_contacts_t.contacts = &contacts;
        process_contacts_t.box = &box;
        process_contacts_t.V = &vm_inst.find<real>("V");
        process_contacts_t.F = vm_inst.find<vec3r_vector>("F").to_array();
        process_contacts_t.r = r;
        process_contacts_t.free_pairs = &free_pairs;

        precomp_nh_t = precompute_nh_t.tf_impl(module);
        sift_cand_t = sift_candidates_tf_t.tf_impl(module);
        process_cand_t = module.emplace([this]() -> void {
            process_candidates_t();
        });
        process_cont_t = process_contacts_t.tf_impl(module);

        precomp_nh_t.precede(sift_cand_t);
        sift_cand_t.precede(process_cand_t);
        process_cand_t.precede(process_cont_t);
    }

    tf::Task eval_qa_forces_tf::tf_impl(tf::Taskflow &taskflow) {
        return taskflow.composed_of(module);
    }
}