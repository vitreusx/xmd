#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>
#include <xmd/ctx/context.h>
#include <xmd/model/model.h>
#include <xmd/model/loader.h>

namespace xmd {
    class export_pdb: public ctx_aware {
    public:
        std::filesystem::path out_file_path;
        real period;

    public:
        const_array<vec3tr> true_r;
        xmd::model const *ref_model;
        res_map_t const *res_map;
        int *serial;
        real *last_t, *t;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}