#pragma once
#include <vector>
#include <unordered_set>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "types/amino_acid.h"

namespace xmd {
    class model {
    public:
        static model random(int num_res);

        friend model operator+(model const& m1, model const& m2);
        model& operator+=(model const& m2);

        model& apply(Eigen::Affine3f const& T);

        template<typename Random>
        model& morph_into_saw(Random& rand, float res_bond_length,
            float min_res_dist, bool set_geometry);

    public:
        struct residue;
        struct chain;

        struct residue {
            chain *parent_chain;
            amino_acid type;
            Eigen::Vector3d pos;
        };
        std::unordered_set<residue> residues;

        struct chain {
            std::vector<residue*> residues;
        };
        std::unordered_set<chain> chains;

        struct contact {
            residue *r1, *r2;
            double def_dist;
        };
        std::unordered_set<contact> contacts, disulfide_bonds;

        struct angle {
            residue *r1, *r2, *r3;
            double def_theta;
        };
        std::unordered_set<angle> angles;

        struct dihedral {
            residue *r1, *r2, *r3, *r4;
            double def_phi;
        };
        std::unordered_set<dihedral> dihedrals;

        Eigen::Vector3f cell;
    };
}