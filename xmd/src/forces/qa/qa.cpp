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
}