#pragma once
#include <xmd/model/model.h>
#include <xmd/vm/vm.h>
#include <unordered_map>

namespace xmd {
    using res_map_t = std::unordered_map<model::residue*, int>;

    class model_loader: public vm_aware {
    public:
        model* xmd_model;
        explicit model_loader(model* xmd_model);

        void init_from_vm(vm& vm_inst) override;
    };
}