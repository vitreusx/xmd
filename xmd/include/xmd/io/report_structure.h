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
        xmd::model *xmd_model;
        res_map_t *res_map;
        vec3r_array r;
        std::optional<qa::contact_set*> qa_cont;
        std::optional<qa::sync_data_array> sync;
        real *t, *last_t;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}