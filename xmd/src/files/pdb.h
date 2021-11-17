#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Eigen/Core>
#include "model/model.h"

namespace xmd {
    class pdb {
    public:
        explicit pdb(std::istream& is);
        friend std::ostream& operator<<(std::ostream& os, pdb const& p);

        void add_snapshot(xmd::model const& m);

        enum class contact_deriv {
            NONE, FROM_ATOMS, FROM_RESIDUES
        };
        xmd::model to_model(contact_deriv const& deriv) const;

        struct atom;
        struct residue;
        struct chain;
        struct model;

        struct atom {
            std::string name;
            Eigen::Vector3f pos;
            residue *parent_res;
        };

        struct residue {
            chain *parent_chain;
            std::string name;
            std::vector<atom> atoms;
        };

        struct chain {
            model *parent_model;
            std::vector<residue> residues;
        };

        struct model {
            std::vector<chain> chains;
        };
        std::vector<model> models;

        struct disulfide_bond {
            atom *a1, *a2;
            float dist;
        };
        std::vector<disulfide_bond> disulfide_bonds;

        struct link {
            atom *a1, *a2;
            float dist;
        };
        std::vector<link> links;

        struct geometry {
            Eigen::Vector3f cell;
            // TODO: figure out ORIGXn and SCALEn
        };
        geometry geom;
    };
}