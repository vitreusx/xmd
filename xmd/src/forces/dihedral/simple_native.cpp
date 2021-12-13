#include <params/param_file.h>
#include <utils/units.h>
#include "forces/dihedral/simple_native.h"

namespace xmd {

    void eval_snd_forces::operator()() const {
        for (int idx = 0; idx < dihedrals.size; ++idx)
            loop_iter(idx);
    }

    void eval_snd_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        CDH = vm_inst.find_or_add<real>("CDH",
            params["simple native dihedrals"]["CDH"].as<quantity>());

        eval_native_dihedral_forces_base::init_from_vm(vm_inst);
    }

    void eval_snd_forces::loop_iter(int idx) const {
        int i1 = dihedrals.i1[idx], i2 = dihedrals.i2[idx],
            i3 = dihedrals.i3[idx], i4 = dihedrals.i4[idx];
        auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
        auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
        auto x12_23 = cross(r12, r23), x23_34 = cross(r23, r34);

        auto x12_23_rn = norm_inv(x12_23), x23_34_rn = norm_inv(x23_34);
        auto x12_23_u = x12_23 * x12_23_rn, x23_34_u = x23_34 * x23_34_rn;

        auto cos_phi = dot(x12_23_u, x23_34_u);
        auto phi = acos(cos_phi);
        if (dot(x12_23, r34) < 0.0f) phi = -phi;

        auto diff = phi - dihedrals.nat_phi[idx];
        *V += 0.5f * CDH * diff * diff;
        auto dV_dphi = CDH * diff;

        auto r23_n = norm(r23);
        auto dphi_dr1 = -x12_23_u * r23_n * x12_23_rn;
        auto dphi_dr4 = x23_34_u * r23_n * x23_34_rn;
        auto df = (-dphi_dr1 * dot(r12, r23) + dphi_dr4 * dot(r23, r34)) /
                  (r23_n * r23_n);
        auto dphi_dr2 = -dphi_dr1 + df;
        auto dphi_dr3 = -dphi_dr4 - df;

        F[i1] -= dV_dphi * dphi_dr1;
        F[i2] -= dV_dphi * dphi_dr2;
        F[i3] -= dV_dphi * dphi_dr3;
        F[i4] -= dV_dphi * dphi_dr4;
    }

    tf::Task eval_snd_forces::tf_impl(tf::Taskflow &taskflow) const {
        return taskflow.for_each_index(0, dihedrals.size, 1,
            [this](auto idx) -> void { loop_iter(idx); });
    }
}