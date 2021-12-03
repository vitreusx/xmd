#include "io/export_pdb.h"
#include <xmd/files/pdb.h>
#include <xmd/model/model.h>

namespace xmd {
    export_pdb::export_pdb(std::ostream &out_file):
        out_file {out_file} {}

    static Eigen::Vector3d cast_vector(vec3d const& v) {
        return { v.x(), v.y(), v.z() };
    }

    void export_pdb::operator()() const {
        xmd::model xmd_model;
        xmd_model.residues.resize(num_particles);
        xmd_model.chains.resize(num_chains);

        for (int idx = 0; idx < num_chains; ++idx) {
            auto& chain_ = xmd_model.chains[idx] =
                std::make_unique<xmd::model::chain>();

            chain_->chain_idx = idx;
        }

        for (int idx = 0; idx < num_particles; ++idx) {
            auto& res = xmd_model.residues[idx] =
                std::make_unique<xmd::model::residue>();

            res->type = atype[idx];
            res->pos = cast_vector(true_r[idx]);
            res->seq_idx = chain_seq_idx[idx];
            res->parent = xmd_model.chains[chain_idx[idx]].get();

            if ((int)res->parent->residues.size() <= res->seq_idx)
                res->parent->residues.resize(res->seq_idx+1);
            res->parent->residues[res->seq_idx] = res.get();
        }

        pdb pdb_model(xmd_model);
        if (*serial > 1) out_file << '\n';
        out_file << pdb(xmd_model).emit_model((*serial)++);
        out_file.flush();
    };
}