#pragma once
#include <optional>
#include <xmd/vm/vm.h>
#include <xmd/types/vec3.h>
#include <xmd/model/model.h>
#include <xmd/model/loader.h>
#include <xmd/forces/qa/qa.h>

namespace xmd {
    class report_structure: public vm_aware {
    public:
        std::string path_fmt;
        real period, nat_active_thr;

    public:
        xmd::model const *xmd_model;
        res_map_t const *res_map;
        const_array<vec3r> r;
        std::optional<set<qa::contact> const*> qa_cont;
        std::optional<const_array<qa::sync_data>> sync;
        real *t, *last_t;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}