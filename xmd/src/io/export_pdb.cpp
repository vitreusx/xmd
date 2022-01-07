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

    void export_pdb::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        out_file_path = ctx.persistent<std::filesystem::path>(
            "export_pdb_path", params["export pdb"]["path"].as<std::string>());
        period = ctx.persistent<real>("export_pdb_period",
            params["export pdb"]["exec period"].as<quantity>());

        true_r = ctx.var<vector<vec3tr>>("true_r").data();
        ref_model = &ctx.var<xmd::model>("model");
        res_map = &ctx.var<res_map_t>("res_map");
        serial = &ctx.persistent<int>("serial", 1);
        last_t = &ctx.ephemeral<real>("export_pdb_last_t",
            std::numeric_limits<real>::lowest());
        t = &ctx.var<real>("t");
    };
}