#include "io/export_pdb.h"
#include <xmd/files/pdb.h>
#include <xmd/model/model.h>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <fstream>

namespace xmd {
    static Eigen::Vector3d to_eigen_vector(vec3tr const& v) {
        return { v.x(), v.y(), v.z() };
    }

    void export_pdb::operator()() const {
        if (*t - *last_t >= period) {
            auto xmd_model = *ref_model;

            for (int idx = 0; idx < (int)xmd_model.residues.size(); ++idx) {
                auto *ref_res = ref_model->residues[idx].get();
                auto res_idx = (*res_map)[ref_res];
                xmd_model.residues[idx]->pos = to_eigen_vector(true_r[res_idx]);
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

    void export_pdb::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        out_file_path = vm_inst.find_or_emplace<std::filesystem::path>(
            "export_pdb_path", params["export pdb"]["path"].as<std::string>());
        period = vm_inst.find_or_emplace<real>("export_pdb_period",
            params["export pdb"]["exec period"].as<quantity>());

        true_r = vm_inst.find<vec3tr_vector>("true_r").to_array();
        ref_model = &vm_inst.find<xmd::model>("model");
        res_map = &vm_inst.find<res_map_t>("res_map");
        serial = &vm_inst.find_or_emplace<int>("serial", 1);
        last_t = &vm_inst.find_or_emplace<real>("export_pdb_last_t",
            std::numeric_limits<real>::lowest());
        t = &vm_inst.find<real>("t");
    };
}