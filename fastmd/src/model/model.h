#pragma once
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "univ/amino_acid.h"

namespace fastmd {
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
            Eigen::Vector3f pos;
        };
        auto residues();

        struct chain {
            std::vector<residue> residues;
            model to_model();
        };
        std::vector<chain> chains;

        struct contact {
            residue *r1, *r2;
            float def_dist;
        };
        std::vector<contact> contacts, disulfide_bonds;

        struct angle {
            residue *r1, *r2, *r3;
            float def_theta;
        };
        std::vector<angle> angles;

        struct dihedral {
            residue *r1, *r2, *r3, *r4;
            float def_phi;
        };
        std::vector<dihedral> dihedrals;

        struct geometry {
            Eigen::Vector3f cell;
            // TODO: figure out ORIGXn and SCALEn
        };
        geometry geom;
    };
}