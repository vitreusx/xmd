#pragma once
#include "xmd/types/amino_acid.h"
#include "xmd/model/box.h"
#include <vector>
#include <list>
#include <optional>
#include <memory>
#include <xmd/random/rand_gen.h>
#include <xmd/types/scalar.h>
#include <xmd/types/vec3.h>

namespace xmd {
    class model {
    public:
        model() = default;
        model(model const& other);
        model& operator=(model const& other);

        friend model operator+(model const& m1, model const& m2);
        model& operator+=(model const& m2);

        void morph_into_saw(rand_gen& gen, std::optional<true_real> res_bond_length,
            true_real base_res_dens, bool infer_box);

    public:
        struct residue;
        struct chain;

        struct residue {
            chain* parent;
            int seq_idx;
            amino_acid type;
            vec3tr pos;
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

        box model_box;
    };
}
