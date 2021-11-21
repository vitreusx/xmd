#pragma once
#include <Eigen/Geometry>

namespace xmd {
    inline Eigen::Vector3d perpendicular_vector(Eigen::Vector3d const& v) {
        auto perp1 = v.cross(Eigen::Vector3d::UnitX());
        auto perp2 = v.cross(Eigen::Vector3d::UnitY());
        auto& perp = perp1.norm() > perp2.norm() ? perp1 : perp2;
        return perp.normalized();
    }

    template<typename Random>
    void model::morph_into_saw(Random& rand, std::optional<double> res_bond_length,
        double min_res_dist, double res_dens, bool infer_cell) {

        auto vol = residues.size() / res_dens;
        auto cell_a = pow(vol, 1.0/3.0);

        bool loop = true;
        while (loop) {
            for (auto const& xmd_chain: chains) {
                Eigen::Vector3d pos {
                    rand.uniform(-cell_a/2.0, cell_a/2.0),
                    rand.uniform(-cell_a/2.0, cell_a/2.0),
                    rand.uniform(-cell_a/2.0, cell_a/2.0)
                };
                auto dir = rand.sphere();

                for (size_t res_idx = 0; res_idx < xmd_chain->residues.size(); ++res_idx) {
                    auto next = pos;
                    if (res_idx+1 < xmd_chain->residues.size()) {
                        double bond_length;
                        if (res_bond_length) {
                            bond_length = res_bond_length.value();
                        }
                        else {
                            auto pos1 = xmd_chain->residues[res_idx]->pos;
                            auto pos2 = xmd_chain->residues[res_idx+1]->pos;
                            bond_length = (pos2-pos1).norm();
                        }

                        next += dir * bond_length;

                        double spread_angle = rand.uniform(M_PI/12.0, M_PI/2.0);
                        auto spread = Eigen::AngleAxisd(spread_angle, perpendicular_vector(dir));

                        double around_angle = rand.uniform(-M_PI, M_PI);
                        auto around = Eigen::AngleAxisd(around_angle, dir);

                        dir = around * spread * dir;
                    }

                    xmd_chain->residues[res_idx]->pos = pos;
                }
            }

            if (infer_cell) {
                auto min_val = std::numeric_limits<double>::min();
                auto x_max = min_val, y_max = min_val, z_max = min_val;
                auto max_val = std::numeric_limits<double>::max();
                auto x_min = max_val, y_min = max_val, z_min = max_val;

                for (auto const& res: residues) {
                    auto const& p = res->pos;
                    x_min = std::min(x_min, p.x());
                    x_max = std::max(x_max, p.x());
                    y_min = std::min(y_min, p.x());
                    y_max = std::max(y_max, p.x());
                    z_min = std::min(z_min, p.x());
                    z_max = std::max(z_max, p.x());
                }

                auto x_ext = x_max - x_min, y_ext = y_max - y_min,
                     z_ext = z_max - z_min;
                model_box.cell = { x_ext, y_ext, z_ext };

                auto x_ext_inv = 1.0 / x_ext, y_ext_inv = 1.0 / y_ext,
                     z_ext_inv = 1.0 / z_ext;
                model_box.cell_inv = { x_ext_inv, y_ext_inv, z_ext_inv };
            }

            loop = false;
            for (auto& res1: residues) {
                for (auto& res2: residues) {
                    bool viable_for_check =
                        (res1->parent != res2->parent) ||
                        (abs(res1->seq_num - res2->seq_num) >= 3);

                    if (viable_for_check) {
                        auto r12 = model_box.uv(res1->pos, res2->pos);
                        if (r12.norm() < min_res_dist) {
                            loop = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}