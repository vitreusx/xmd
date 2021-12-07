#include "dynamics/lang_pc.h"
#include <xmd/utils/units.h>

namespace xmd {
    void lang_pc_step::operator()() const {
        auto local_gen = *gen;
        real noise_factor = sqrt(2.0 * kB * temperature);
        true_real dt_inv = 1.0/dt;

        for (int idx = 0; idx < num_particles; ++idx) {
            auto gamma = gamma_factor * mass[idx];
            auto noise_sd = noise_factor * sqrt(gamma);

            auto [noise_x, noise_y] = local_gen.normalx2<real>();
            auto noise_z = local_gen.normal<real>();
            auto noise = vec3r(noise_x, noise_y, noise_z);

            auto a_ = F[idx] * mass_inv[idx] - gamma_factor * v[idx] +
                noise_sd * noise * mass_inv[idx];

            vec3tr error = y2[idx] - a_ * (dt*dt/2.0);
            y0[idx] -= 3.0/16.0 * error;
            y1[idx] -= 251.0/360.0 * error;
            y2[idx] -= 1.0 * error;
            y3[idx] -= 11.0/18.0 * error;
            y4[idx] -= 1.0/6.0 * error;
            y5[idx] -= 1.0/60.0 * error;

            y0[idx] += y1[idx] + y2[idx] + y3[idx] + y4[idx] + y5[idx];
            y1[idx] += 2.0*y2[idx] + 3.0*y3[idx] + 4.0*y4[idx] + 5.0*y5[idx];
            y2[idx] += 3.0*y3[idx] + 6.0*y4[idx] + 10.0*y5[idx];
            y3[idx] += 4.0*y4[idx] + 10.0*y5[idx];
            y4[idx] += 5.0*y5[idx];

            r[idx] = y0[idx];
            v[idx] = y1[idx]*dt_inv;
        }

        *t = (*true_t += dt);
        *gen = local_gen;
    }

    void lang_pc_step::bind_to_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        t = &vm_inst.find<real>("t");
        num_particles = vm_inst.find<int>("num_particles");
        mass = vm_inst.find<vector<real>>("mass").to_array();
        gen = &vm_inst.find<xorshift64>("gen");

        mass_inv = vm_inst.find_or<vector<real>>("mass_inv", [&]() -> auto& {
            auto& mass_inv_ = vm_inst.emplace<vector<real>>("mass_inv", num_particles);
            for (int idx = 0; idx < num_particles; ++idx)
                mass_inv_[idx] = (real)1.0 / mass[idx];
            return mass_inv_;
        }).to_array();
        y0 = vm_inst.find_or<vec3tr_vector>("y0", [&]() -> auto& {
            auto& y0_ = vm_inst.emplace<vec3tr_vector>("y0", num_particles);
            for (int idx = 0; idx < num_particles; ++idx)
                y0_[idx] = y0[idx];
            return y0_;
        }).to_array();
        y1 = vm_inst.find_or_emplace<vec3tr_vector>("y1",
            num_particles).to_array();
        y2 = vm_inst.find_or_emplace<vec3tr_vector>("y2",
            num_particles).to_array();
        y3 = vm_inst.find_or_emplace<vec3tr_vector>("y3",
            num_particles).to_array();
        y4 = vm_inst.find_or_emplace<vec3tr_vector>("y4",
            num_particles).to_array();
        y5 = vm_inst.find_or_emplace<vec3tr_vector>("y5",
            num_particles).to_array();
        true_t = &vm_inst.find_or_emplace<true_real>("true_t", *t);
    }
}