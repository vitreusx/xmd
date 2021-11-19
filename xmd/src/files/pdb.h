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
            size_t seq_num;
            Eigen::Vector3f pos;
            residue *parent_res;
        };
        std::unordered_map<size_t, atom> atoms;

        struct residue {
            chain *parent_chain;
            size_t seq_num;
            std::string name;
            std::vector<atom*> atoms;
        };
        std::unordered_map<size_t, residue> residues;

        struct chain {
            model *parent_model;
            char chain_id;
            std::vector<residue*> residues;
        };
        std::unordered_map<char, chain> chains;

        struct model {
            size_t serial;
            std::vector<residue*> residues;
            std::vector<chain*> chains;
        };
        std::unordered_map<size_t, model> models;

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

        Eigen::Vector3f cryst1;
    };
}