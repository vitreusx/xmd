#include "forces/dihedral/native.h"

namespace xmd {
    void eval_native_dihedral_forces_base::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");

        dihedrals = vm_inst.find_or<vector<nat_dih>>("native_dihedrals",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& dihedrals_ = vm_inst.emplace<vector<nat_dih>>(
                    "native_dihedrals");

                for (auto const& dihedral: xmd_model.dihedrals) {
                    if (dihedral.phi.has_value()) {
                        auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                            i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];
                        auto nat_phi = (real)dihedral.phi.value();

                        dihedrals_.emplace_back(i1, i2, i3, i4, nat_phi);
                    }
                }

                return dihedrals_;
            }).view();
    }
}