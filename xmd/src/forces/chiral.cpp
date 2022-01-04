#include <xmd/forces/chiral.h>
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/utils/convert.h>

namespace xmd {
    void eval_chiral_forces::operator()() const {
        for (int idx = 0; idx < quads.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_chiral_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        e_chi = vm_inst.find_or_emplace<real>("e_chi",
            params["chirality"]["e_chi"].as<quantity>());

        r = vm_inst.find<vector<vec3r>>("r").data();
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");

        quads = vm_inst.find_or<vector<chiral_quad>>("chiral_quads",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& quads_ = vm_inst.emplace<vector<chiral_quad>>(
                    "chiral_quads");

                for (auto const& dihedral: xmd_model.dihedrals) {
                    auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                        i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];

                    vec3r nat_r1 = dihedral.res1->pos, nat_r2 = dihedral.res2->pos,
                        nat_r3 = dihedral.res3->pos, nat_r4 = dihedral.res4->pos;

                    auto nat_r12 = nat_r2 - nat_r1, nat_r23 = nat_r3 - nat_r2,
                        nat_r34 = nat_r4 - nat_r3;

                    auto nat_factor = ipow<3>(norm_inv(nat_r23));
                    auto nat_chir = dot(nat_r12, cross(nat_r23, nat_r34)) * nat_factor;

                    quads_.emplace_back(i1, i2, i3, i4, nat_factor, nat_chir);
                }

                return quads_;
            }).view();
    }

    void eval_chiral_forces::iter(int idx) const {
        auto quad = quads[idx];

        auto i1 = quad.i1(), i2 = quad.i2(), i3 = quad.i3(), i4 = quad.i4();
        auto nat_chir = quad.nat_chir();
        auto nat_factor = quad.nat_factor();

        auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
        auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
        auto x12_23 = cross(r12, r23), x12_34 = cross(r12, r34),
            x23_34 = cross(r23, r34);

        auto chir = dot(r12, x23_34) * nat_factor;
        auto chir_diff = chir - nat_chir;

        *V += 0.5f * e_chi * chir_diff * chir_diff;

        auto f = e_chi * chir_diff * nat_factor;
        F[i1] += f * x12_23;
        F[i2] -= f * (x12_34 + x23_34);
        F[i3] += f * (x12_23 + x12_34);
        F[i4] -= f * x12_23;
    }

    void eval_chiral_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < quads.size(); ++idx) {
            iter(idx);
        }
    }
}