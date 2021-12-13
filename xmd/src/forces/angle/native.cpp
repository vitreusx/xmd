#include "forces/angle/native.h"
#include <xmd/model/model.h>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_native_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size; ++idx)
            loop_iter(idx);
    }

    void eval_native_angle_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        k = vm_inst.find_or_add<real>("nat_ang_k",
            params["native angles"]["k"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");

        angles = vm_inst.find_or<native_angle_vector>("native_angles",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                int num_nat_ang = 0;
                for (auto const& angle: xmd_model.angles) {
                    if (angle.theta.has_value())
                        num_nat_ang += 1;
                }

                auto& angles_vec = vm_inst.emplace<native_angle_vector>(
                    "native_angles", num_nat_ang);

                int ang_idx = 0;
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");
                for (auto const& angle: xmd_model.angles) {
                    if (angle.theta.has_value()) {
                        angles_vec.i1[ang_idx] = res_map[angle.res1];
                        angles_vec.i2[ang_idx] = res_map[angle.res2];
                        angles_vec.i3[ang_idx] = res_map[angle.res3];
                        angles_vec.nat_theta[ang_idx] = (real)angle.theta.value();

                        ++ang_idx;
                    }
                }

                return angles_vec;
            }).to_span();
    }

    tf::Task eval_native_angle_forces::tf_impl(tf::Taskflow &taskflow) const {
        return taskflow.for_each_index(0, angles.size, 1,
            [this](auto idx) -> void { loop_iter(idx); });
    }

    void eval_native_angle_forces::loop_iter(int idx) const {
        int i1 = angles.i1[idx], i2 = angles.i2[idx], i3 = angles.i3[idx];
        auto nat_theta = angles.nat_theta[idx];
        auto r1 = r[i1], r2 = r[i2], r3 = r[i3];

        auto r12 = r2 - r1, r23 = r3 - r2;

        auto x12_23 = cross(r12, r23);
        auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);

        auto dtheta_dr1 = unit(cross(r12, x12_23)) * r12_rn;
        auto dtheta_dr3 = unit(cross(r23, x12_23)) * r23_rn;
        auto dtheta_dr2 = -dtheta_dr1-dtheta_dr3;

        auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
        auto theta = acos(cos_theta);

        auto dtheta = theta - nat_theta;
        *V += (real)0.5 * k * dtheta * dtheta;
        auto dV_dtheta = k * dtheta;

        F[i1] -= dV_dtheta * dtheta_dr1;
        F[i2] -= dV_dtheta * dtheta_dr2;
        F[i3] -= dV_dtheta * dtheta_dr3;
    }

    native_angle_span native_angle_vector::to_span() {
        native_angle_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.nat_theta = nat_theta.to_array();
        span.size = size;
        return span;
    }

    native_angle_vector::native_angle_vector(int n):
        i1{n}, i2{n}, i3{n}, nat_theta{n}, size{n} {};
}