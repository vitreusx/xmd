#include "forces/dihedral/native.h"

namespace xmd {
    void eval_native_dihedral_forces_base::declare_vars(context& ctx) {
        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.per_thread().var<vector<vec3r>>("F").data();
        V = &ctx.per_thread().var<real>("V");

        dihedrals = ctx.persistent<vector<nat_dih>>("native_dihedrals",
            lazy([&]() -> auto {
                auto& xmd_model = ctx.var<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = ctx.var<res_map_t>("res_map");

                vector<nat_dih> dihedrals_;

                for (auto const& dihedral: xmd_model.dihedrals) {
                    if (dihedral.phi.has_value()) {
                        auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                            i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];
                        auto nat_phi = (real)dihedral.phi.value();

                        dihedrals_.emplace_back(i1, i2, i3, i4, nat_phi);
                    }
                }

                return dihedrals_;
            })).view();
    }
}