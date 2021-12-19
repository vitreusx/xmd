#include "forces/walls/solid.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_solid_wall_forces::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        eps = vm_inst.find_or_emplace<real>("solid_wall_eps",
            params["solid walls"]["depth"].as<quantity>());
        cutoff = vm_inst.find_or_emplace<real>("solid_wall_cutoff",
            params["solid walls"]["cutoff"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        walls = vm_inst.find<vector<plane>>("solid_walls").to_span();
        wall_F = vm_inst.find_or_emplace<vec3r_vector>("solid_wall_F",
            walls.size()).to_array();
        V = &vm_inst.find<real>("V");
        num_particles = vm_inst.find<int>("num_particles");
        box = &vm_inst.find<box<vec3r>>("box");
    }

    void eval_solid_wall_forces::omp_async() const {
#pragma omp for nowait
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::iter(int idx) const {
        auto part_r = box->warp_to_box(r[idx]);
        for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
            auto wall = walls[wall_idx];
            auto x = signed_dist(part_r, wall);
            if (x < cutoff) {
                auto V_ = eps / ((real)9.0 * ipow<9>(x));
                auto dV_dx = V_ * ((real)(-9.0) * x);
                *V += V_;
                F[idx] += dV_dx * wall.normal;
                wall_F[wall_idx] -= dV_dx * wall.normal;
            }
        }
    }
}