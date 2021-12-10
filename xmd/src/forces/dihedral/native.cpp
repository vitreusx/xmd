#include "forces/dihedral/native.h"

namespace xmd {
    void eval_native_dihedral_forces_base::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");

        dihedrals = vm_inst.find_or<native_dihedral_vector>("native_dihedrals",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                int num_nat_dih = 0;
                for (auto const& dihedral: xmd_model.dihedrals) {
                    if (dihedral.phi.has_value())
                        num_nat_dih += 1;
                }

                auto& dihedrals_vec = vm_inst.emplace<native_dihedral_vector>(
                    "native_dihedrals", num_nat_dih);

                int dih_idx = 0;
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");
                for (auto const& dihedral: xmd_model.dihedrals) {
                    if (dihedral.phi.has_value()) {
                        dihedrals_vec.i1[dih_idx] = res_map[dihedral.res1];
                        dihedrals_vec.i2[dih_idx] = res_map[dihedral.res2];
                        dihedrals_vec.i3[dih_idx] = res_map[dihedral.res3];
                        dihedrals_vec.i4[dih_idx] = res_map[dihedral.res4];
                        dihedrals_vec.nat_phi[dih_idx] = (real)dihedral.phi.value();

                        ++dih_idx;
                    }
                }

                return dihedrals_vec;
            }).to_span();
    }

    native_dihedral_span native_dihedral_vector::to_span() {
        native_dihedral_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.i4 = i4.to_array();
        span.nat_phi = nat_phi.to_array();
        span.size = size;
        return span;
    }

    native_dihedral_vector::native_dihedral_vector(int n):
        i1{n}, i2{n}, i3{n}, i4{n}, nat_phi{n}, size{n} {};
}