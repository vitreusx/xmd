#include "nl/verify.h"

namespace xmd::nl {
    void verify::operator()() const {
        if (*first_time) {
            *first_time = false;
            *invalid = true;
        }
        else {
            real max_r_disp = 0.0f;
            for (int idx = 0; idx < num_particles; ++idx) {
                auto r_ = r[idx], orig_r_ = orig_r[idx];
                auto dr = box->ray(r_, orig_r_);
                max_r_disp = max(max_r_disp, norm(dr));
            }

            auto box_disp = l1_norm(box->cell - orig_box->cell);

            real total_disp = 2.0f * max_r_disp + box_disp;
            if (total_disp > *pad)
                *invalid = true;
        }
    }

    void verify::init_from_vm(vm& vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        orig_r = vm_inst.find<vec3r_vector>("orig_r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        orig_box = &vm_inst.find<xmd::box<vec3r>>("orig_box");
        invalid = &vm_inst.find_or_emplace<bool>("invalid", true);
        first_time = &vm_inst.find_or_emplace<bool>("first_time", true);
        num_particles = vm_inst.find<int>("num_particles");
        pad = &vm_inst.find<real>("pad");
    }
}