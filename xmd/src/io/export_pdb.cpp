#include "io/export_pdb.h"
#include <xmd/files/pdb.h>
#include <xmd/model/model.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <fstream>
#include <xmd/utils/convert.h>

namespace xmd {
    void export_pdb::operator()() const {
        if (*t - *last_t >= period) {
            auto xmd_model = *ref_model;

            for (int idx = 0; idx < (int)xmd_model.residues.size(); ++idx) {
                auto *ref_res = ref_model->residues[idx].get();
                auto res_idx = res_map->at(ref_res);
                xmd_model.residues[idx]->pos = true_r[res_idx];
            }

            pdb pdb_model(xmd_model);

            auto mode = *serial == 1 ? std::ios::out : std::ios::app;
            auto out_file = std::ofstream(out_file_path, mode);

            if (*serial > 1) out_file << '\n';
            out_file << pdb(xmd_model).emit_model((*serial)++);
            out_file.flush();

            *last_t = *t;
        }
    }
}