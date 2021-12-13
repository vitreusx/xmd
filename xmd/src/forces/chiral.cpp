#include <xmd/forces/chiral.h>
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_chiral_forces::operator()() const {
        for (int idx = 0; idx < quads.size; ++idx)
            loop_iter(idx);
    }

    void eval_chiral_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        e_chi = vm_inst.find_or_emplace<real>("e_chi",
            params["chirality"]["e_chi"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");

        quads = vm_inst.find_or<chiral_quad_vector>("chiral_quads",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& quads_vec = vm_inst.emplace<chiral_quad_vector>(
                    "chiral_quads", xmd_model.dihedrals.size());

                auto quads_idx = 0;
                for (auto const& dihedral: xmd_model.dihedrals) {
                    quads.i1[quads_idx] = res_map[dihedral.res1];
                    quads.i2[quads_idx] = res_map[dihedral.res2];
                    quads.i3[quads_idx] = res_map[dihedral.res3];
                    quads.i4[quads_idx] = res_map[dihedral.res4];

                    auto const& cast_vec = [](Eigen::Vector3d const& v) -> vec3r {
                        return { (real)v.x(), (real)v.y(), (real)v.z() };
                    };

                    auto nat_r1 = cast_vec(dihedral.res1->pos);
                    auto nat_r2 = cast_vec(dihedral.res2->pos);
                    auto nat_r3 = cast_vec(dihedral.res3->pos);
                    auto nat_r4 = cast_vec(dihedral.res4->pos);

                    auto nat_r12 = nat_r2 - nat_r1, nat_r23 = nat_r3 - nat_r2,
                        nat_r34 = nat_r4 - nat_r3;

                    auto nat_factor = ipow<3>(norm_inv(nat_r23));
                    auto nat_chir = dot(nat_r12, cross(nat_r23, nat_r34)) * nat_factor;

                    quads.nat_factor[quads_idx] = nat_factor;
                    quads.nat_chir[quads_idx] = nat_chir;

                    ++quads_idx;
                }

                return quads_vec;
            }).to_span();
    }

    void eval_chiral_forces::loop_iter(int idx) const {
        auto i1 = quads.i1[idx], i2 = quads.i2[idx], i3 = quads.i3[idx],
            i4 = quads.i4[idx];
        auto nat_chir = quads.nat_chir[idx];
        auto nat_factor = quads.nat_factor[idx];

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

    tf::Task eval_chiral_forces::tf_impl(tf::Taskflow &taskflow) const {
        return taskflow.for_each_index(0, quads.size, 1,
            [=](auto idx) -> void { loop_iter(idx); });
    }

    chiral_quad_vector::chiral_quad_vector(int n):
        i1{n}, i2{n}, i3{n}, i4{n}, nat_chir{n}, nat_factor {n} {}

    chiral_quad_span chiral_quad_vector::to_span() {
        chiral_quad_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.i4 = i4.to_array();
        span.nat_factor = nat_factor.to_array();
        span.nat_chir = nat_chir.to_array();
        span.size = size;
        return span;
    };
}