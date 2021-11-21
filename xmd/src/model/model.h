#pragma once
#include <vector>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "types/amino_acid.h"
#include "box.h"
#include <optional>
#include <memory>

namespace xmd {
    class model {
    public:
        model() = default;
        model(model const& other);
        model& operator=(model const& other);

        friend model operator+(model const& m1, model const& m2);
        model& operator+=(model const& m2);

        template<typename Random>
        inline void morph_into_saw(Random& rand, std::optional<double> res_bond_length,
            double min_res_dist, double res_dens, bool infer_cell);

    public:
        struct residue;
        struct chain;

        struct residue {
            chain* parent;
            int seq_num;
            amino_acid type;
            Eigen::Vector3d pos;
        };
        std::vector<std::unique_ptr<residue>> residues;

        struct chain {
            std::vector<residue*> residues;
        };
        std::vector<std::unique_ptr<chain>> chains;

        struct contact {
            residue *res1, *res2;
            double length;
        };
        std::vector<contact> contacts, disulfide_bonds;

        struct angle {
            residue *res1, *res2, *res3;
            double theta;
        };
        std::vector<angle> angles;

        struct dihedral {
            residue *res1, *res2, *res3, *res4;
            double phi;
        };
        std::vector<dihedral> dihedrals;

        box model_box;
    };
}

#include "detail/model.inl"
