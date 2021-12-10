#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>
#include <xmd/vm/vm.h>
#include <xmd/model/model.h>
#include <xmd/model/loader.h>

namespace xmd {
    class export_pdb: public vm_aware {
    public:
        std::filesystem::path out_file_path;

    public:
        vec3tr_array true_r;
        xmd::model *ref_model;
        res_map_t *res_map;
        int *serial;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}