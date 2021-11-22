#pragma once
#include "xmd/model/model.h"
#include "xmd/files/param_file.h"
#include <Eigen/Core>
#include <vector>
#include <string>

namespace xmd {
    class pdb {
    public:
        pdb(pdb const& other);
        pdb& operator=(pdb const& other);

        explicit pdb(std::istream& is);
        explicit pdb(xmd::model const& xmd_model);

        friend std::ostream& operator<<(std::ostream& os, pdb const& p);

        void add_contacts(params::amino_acids const& amino_acids,
            bool all_atoms = true);

        enum class contact_deriv {
            NONE, FROM_ATOMS, FROM_RESIDUES
        };
        xmd::model to_model() const;

        struct atom;
        struct residue;
        struct chain;
        struct model;

        struct atom {
            std::string name;
            size_t serial;
            Eigen::Vector3d pos;
            residue *parent_res;
        };

        struct residue {
            chain *parent_chain;
            size_t seq_num;
            std::string name;
            std::vector<atom*> atoms;

            atom *find_by_name(std::string const& name) const;
        };

        struct chain {
            char chain_id;
            std::unordered_map<size_t, atom> atoms;
            std::unordered_map<size_t, residue> residues;
            std::vector<residue*> order;
            size_t ter_serial;
        };
        std::unordered_map<char, chain> chains;

        struct disulfide_bond {
            size_t serial;
            atom *a1, *a2;
            double length;
        };
        std::unordered_map<size_t, disulfide_bond> disulfide_bonds;

        struct link {
            atom *a1, *a2;
            double length;
        };
        std::vector<link> links;

        Eigen::Vector3d cryst1;

    private:
        chain *find_chain(char chain_id);
        residue *find_res(chain& c, size_t seq_num);
    };
}