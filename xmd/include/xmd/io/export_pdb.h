#pragma once
#include <ostream>
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>

namespace xmd {
    class export_pdb {
    public:
        vec3tr_array true_r;
        array<amino_acid> atype;
        array<int8_t> chain_idx;
        array<int> chain_seq_idx;
        int num_particles, num_chains, *serial;

        std::ostream& out_file;

        explicit export_pdb(std::ostream& out_file);

    public:
        void operator()() const;
    };
}