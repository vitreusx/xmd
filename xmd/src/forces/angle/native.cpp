#include "forces/angle/native.h"
#include <xmd/model/model.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {
    void eval_native_angle_forces::operator()() const {
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_native_angle_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        k = ctx.persistent<real>("nat_ang_k",
            params["native angles"]["k"].as<quantity>());

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");

        angles = ctx.persistent<vector<nat_ang>>("native_angles",
            lazy([&]() -> auto {
                auto& xmd_model = ctx.var<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = ctx.var<res_map_t>("res_map");

                vector<nat_ang> angles_;

                for (auto const& angle: xmd_model.angles) {
                    if (angle.theta.has_value()) {
                        auto i1 = res_map[angle.res1], i2 = res_map[angle.res2],
                            i3 = res_map[angle.res3];
                        auto nat_theta = (real)angle.theta.value();

                        angles_.emplace_back(i1, i2, i3, nat_theta);
                    }
                }

                return angles_;
            })).view();
    }

    void eval_native_angle_forces::iter(int idx) const {
        auto angle = angles[idx];

        int i1 = angle.i1(), i2 = angle.i2(), i3 = angle.i3();
        auto nat_theta = angle.nat_theta();
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

    void eval_native_angle_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < angles.size(); ++idx) {
            iter(idx);
        }
    }
}