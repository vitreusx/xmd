#include "model/loader.h"
#include <xmd/types/vec3.h>

namespace xmd {
    static vec3r cast_v(Eigen::Vector3d const& v) {
        return { (real)v.x(), (real)v.y(), (real)v.z() };
    }

    model_loader::model_loader(model *xmd_model):
        xmd_model{xmd_model} {}

    void model_loader::init_from_vm(vm &vm_inst) {
        auto& stored_model = vm_inst.find_or_add<model>("model", *xmd_model);
        auto& num_particles = vm_inst.find_or_emplace<int>(
            "num_particles", stored_model.residues.size());

        auto& res_map = vm_inst.find_or_emplace<res_map_t>("res_map");

        vm_inst.find_or<vec3r_vector>("r", [&]() -> auto& {
            auto& r = vm_inst.emplace<vec3r_vector>("r", num_particles);
            int res_idx = 0;
            for (auto const& res: stored_model.residues) {
                res_map[res.get()] = res_idx;
                r[res_idx] = cast_v(res->pos);
                res_idx++;
            }
            return r;
        });

        vm_inst.find_or<vector<amino_acid>>("atype", [&]() -> auto& {
            auto& atype = vm_inst.emplace<vector<amino_acid>>("atype", num_particles);
            for (auto const& res: stored_model.residues) {
                atype[res_map[res.get()]] = res->type;
            }

            return atype;
        });

        vm_inst.find_or_emplace<vector<real>>("mass", num_particles, (real)1.0);

        vm_inst.find_or<xmd::box<vec3r>>("box", [&]() -> auto& {
            xmd::box<vec3r> box;
            box.cell = cast_v(stored_model.model_box.cell);
            box.cell_inv = cast_v(stored_model.model_box.cell_inv);
            return vm_inst.emplace<xmd::box<vec3r>>("box", box);
        });
    }
}