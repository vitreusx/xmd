#pragma once
#include "xmd/types/amino_acid.h"
#include "xmd/model/box.h"
#include <vector>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <optional>
#include <memory>
#include <random>
#include <xmd/random/sphere_dist.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class model {
    public:
        model() = default;
        model(model const& other);
        model& operator=(model const& other);

        friend model operator+(model const& m1, model const& m2);
        model& operator+=(model const& m2);

        template<typename Generator>
        inline void morph_into_saw(Generator& g, std::optional<true_real> res_bond_length,
            true_real base_res_dens, bool infer_box) {

            auto vol = (true_real)residues.size() / base_res_dens;
            auto cell_a = cbrt(vol);

            auto cell_coord_dist = std::uniform_real_distribution<true_real>(
                -cell_a/2.0, cell_a/2.0);
            auto dir_dist = xmd::sphere_dist<Eigen::Vector3d, true_real>();
            auto spread_dist = std::uniform_real_distribution<true_real>(
                M_PI/12.0, M_PI/2.0);
            auto around_dist = std::uniform_real_distribution<true_real>(
                -M_PI, M_PI);

            for (auto const& xmd_chain: chains) {
                Eigen::Vector3d pos {
                    cell_coord_dist(g),
                    cell_coord_dist(g),
                    cell_coord_dist(g)
                };
                auto dir = dir_dist(g);

                for (size_t res_idx = 0; res_idx < xmd_chain->residues.size(); ++res_idx) {
                    auto next = pos;
                    if (res_idx+1 < xmd_chain->residues.size()) {
                        true_real bond_length;
                        if (res_bond_length) {
                            bond_length = res_bond_length.value();
                        }
                        else {
                            auto pos1 = xmd_chain->residues[res_idx]->pos;
                            auto pos2 = xmd_chain->residues[res_idx+1]->pos;
                            bond_length = (pos2-pos1).norm();
                        }

                        next += dir * bond_length;

                        true_real spread_angle = spread_dist(g);
                        auto spread = Eigen::AngleAxisd(spread_angle,
                            any_perpendicular(dir));

                        true_real around_angle = around_dist(g);
                        auto around = Eigen::AngleAxisd(around_angle, dir);

                        dir = (around * spread * dir).normalized();
                    }

                    xmd_chain->residues[res_idx]->pos = pos;
                    pos = next;
                }
            }

            if (infer_box) {
                auto min_val = std::numeric_limits<true_real>::min();
                auto x_max = min_val, y_max = min_val, z_max = min_val;
                auto max_val = std::numeric_limits<true_real>::max();
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

                auto x_ext_inv = 1.0 / x_ext, y_ext_inv = 1.0 / y_ext,
                    z_ext_inv = 1.0 / z_ext;

                model_box.cell = { x_ext, y_ext, z_ext };
                model_box.cell_inv = { x_ext_inv, y_ext_inv, z_ext_inv };
            }
        }

    public:
        struct residue;
        struct chain;

        struct residue {
            chain* parent;
            int seq_idx;
            amino_acid type;
            Eigen::Vector3d pos;
        };
        std::vector<std::unique_ptr<residue>> residues;

        struct chain {
            int chain_idx;
            std::vector<residue*> residues;
        };
        std::vector<std::unique_ptr<chain>> chains;

        enum contact_type {
            UNKNOWN, BACK_BACK, BACK_SIDE, SIDE_BACK, SIDE_SIDE, NAT_SS
        };

        struct contact {
            residue *res1, *res2;
            true_real length;
            contact_type type;
        };
        std::vector<contact> contacts;

        struct tether {
            residue *res1, *res2;
            std::optional<true_real> length;
        };
        std::vector<tether> tethers;

        struct angle {
            residue *res1, *res2, *res3;
            std::optional<true_real> theta;
        };
        std::vector<angle> angles;

        struct dihedral {
            residue *res1, *res2, *res3, *res4;
            std::optional<true_real> phi;
        };
        std::vector<dihedral> dihedrals;

        box<Eigen::Vector3d> model_box;

    private:
        static inline Eigen::Vector3d any_perpendicular(Eigen::Vector3d const& v) {
            auto perp1 = v.cross(Eigen::Vector3d::UnitX());
            auto perp2 = v.cross(Eigen::Vector3d::UnitY());
            auto& perp = perp1.norm() > perp2.norm() ? perp1 : perp2;
            return perp.normalized();
        }
    };
}
