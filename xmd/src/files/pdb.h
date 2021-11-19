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
        explicit pdb(xmd::model const& m);
        friend std::ostream& operator<<(std::ostream& os, pdb const& p);

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
            size_t serial;
            std::unordered_map<size_t, Eigen::Vector3d> pos_for_model;
            residue *parent_res;
        };
        std::unordered_map<size_t, atom> atoms;

        struct residue {
            chain *parent_chain;
            size_t seq_num;
            std::string name;
            std::vector<atom*> atoms;

            atom *find_by_name(std::string const& name);
        };
        std::unordered_map<size_t, residue> residues;

        struct chain {
            char chain_id;
            std::unordered_map<size_t, atom> atoms;
            std::unordered_map<size_t, residue> residues;
            std::vector<residue*> order;
        };
        std::unordered_map<char, chain> chains;

        std::vector<size_t> models;

        struct disulfide_bond {
            size_t serial;
            atom *a1, *a2;
            double dist;
        };
        std::unordered_map<size_t, disulfide_bond> disulfide_bonds;

        struct link {
            atom *a1, *a2;
            double dist;
        };
        std::vector<link> links;

        Eigen::Vector3d cryst1;

    private:
        chain *find_chain(char chain_id);
        residue *find_res(chain& c, size_t seq_num);
    };
}