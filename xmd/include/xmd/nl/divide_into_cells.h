#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "data.h"
#include <xmd/vm/vm.h>
#include <xmd/model/native_contacts.h>

namespace xmd::nl {
    class divide_into_cells: public vm_aware {
    public:
        vec3r_array r;
        box<vec3r> *box;
        nl_data *data;
        int num_particles;
        real *cutoff, *pad;
        bool *invalid;
        native_contact_span nat_cont;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;

    private:
        void prepare(spatial_data& sd) const;
        void assign(spatial_data& sd, int item_idx, int cell_idx) const;
        void derive_rest(spatial_data& sd) const;
    };
}