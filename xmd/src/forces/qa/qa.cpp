#include "forces/qa/qa.h"

namespace xmd::qa {
    void eval_qa_forces::init_from_vm(vm &vm_inst) {
        precompute_nh_t.init_from_vm(vm_inst);

        auto& free_pairs = vm_inst.find_or_emplace<free_pair_set>("qa_free_pairs");
        auto& candidates = vm_inst.find_or_emplace<candidate_list>("qa_candidates");
        auto& contacts = vm_inst.find_or_emplace<contact_set>("qa_contacts");

        auto sync = vm_inst.find<sync_data_vector>("sync").to_array();
        auto& t = vm_inst.find<real>("t");
        auto& box = vm_inst.find<xmd::box<vec3r>>("box");
        auto r = vm_inst.find<vec3r_vector>("r").to_array();

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
        process_candidates_t.t = &t;

        process_contacts_t.sync = sync;
        process_contacts_t.contacts = &contacts;
        process_contacts_t.box = &box;
        process_contacts_t.V = &vm_inst.find<real>("V");
        process_contacts_t.F = vm_inst.find<vec3r_vector>("F").to_array();
        process_contacts_t.r = r;
    }

    void eval_qa_forces::operator()() const {
        precompute_nh_t();
        sift_candidates_t();
        process_candidates_t();
        process_contacts_t();
    }
}