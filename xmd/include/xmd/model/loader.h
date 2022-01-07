#pragma once
#include <xmd/model/model.h>
#include <xmd/ctx/context.h>
#include <unordered_map>

namespace xmd {
    using res_map_t = std::unordered_map<model::residue*, int>;

    class model_loader: public ctx_aware {
    public:
        void declare_vars(context& ctx) override;
    };
}