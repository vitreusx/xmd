#include "forces/qa/qa.h"

namespace xmd::qa {
    void eval_qa_forces::declare_vars(context& ctx) {
        ctx.persistent<set<free_pair>>("qa_free_pairs");
        ctx.persistent<set<candidate>>("qa_candidates");
        ctx.persistent<set<contact>>("qa_contacts");

        ctx.persistent<vector<sync_data>>("sync", lazy([&]() -> auto {
            auto num_particles = ctx.var<int>("num_particles");
            vector<sync_data> sync_vec_(num_particles);

            auto& aa_data_ = ctx.var<amino_acid_data>(
                "amino_acid_data");
            auto& atype = ctx.var<vector<amino_acid>>("atype");

            for (int idx = 0; idx < num_particles; ++idx) {
                auto const& lim = aa_data_[atype[idx]].limits;
                sync_vec_.emplace_back((int8_t)lim.back, (int8_t)lim.side_all,
                    (int8_t)lim.side_polar, (int8_t)lim.side_hydrophobic);
            }

            return sync_vec_;
        }));

        precompute_nh_t.declare_vars(ctx);
        sift_candidates_t.declare_vars(ctx);
        process_candidates_t.declare_vars(ctx);
        process_contacts_t.declare_vars(ctx);
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

    void update_qa::declare_vars(context& ctx) {
        update_free_pairs_.declare_vars(ctx);
    }

    void update_qa::operator()() const {
        update_free_pairs_();
    }
}