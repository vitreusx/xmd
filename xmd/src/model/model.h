#pragma once
#include <vector>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "types/amino_acid.h"

namespace xmd {
    class model {
    public:
        model() = default;
        model(model const& other);
        model& operator=(model const& other);

        friend model operator+(model const& m1, model const& m2);
        model& operator+=(model const& m2);

        template<typename Random>
        model& morph_into_saw(Random& rand, float res_bond_length,
            float min_res_dist, bool infer_cell);

    public:
        struct residue {
            amino_acid type;
            Eigen::Vector3d pos;
        };
        std::list<residue> residues;

        using residue_ref = std::list<model::residue>::iterator;

        struct chain {
            std::vector<residue_ref> residues;
        };
        std::list<chain> chains;

        struct contact {
            residue_ref res1, res2;
            double def_length;
        };
        std::list<contact> contacts, disulfide_bonds;

        struct angle {
            residue_ref res1, res2, res3;
            double def_theta;
        };
        std::list<angle> angles;

        struct dihedral {
            residue_ref res1, res2, res3, res4;
            double def_phi;
        };
        std::list<dihedral> dihedrals;

        Eigen::Vector3d cell;
    };
}