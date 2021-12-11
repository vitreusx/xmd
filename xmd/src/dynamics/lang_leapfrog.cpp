#include "dynamics/lang_leapfrog.h"
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {
    void lang_leapfrog_step::operator()() const {
        auto local_gen = *gen;
        real noise_factor = sqrt(2.0 * kB * temperature);

        for (int idx = 0; idx < num_particles; ++idx) {
            auto gamma = gamma_factor * mass[idx];
            auto noise_sd = noise_factor * sqrt(gamma);

            auto [noise_x, noise_y] = local_gen.normalx2<real>();
            auto noise_z = local_gen.normal<real>();
            auto noise = vec3r(noise_x, noise_y, noise_z);

            vec3tr vel_invariant_term = mass_inv[idx] * (F[idx] + noise_sd * noise);
            vec3tr cur_v = (true_prev_v[idx] + 0.5 * (prev_a[idx] -
                vel_invariant_term * dt)) / (1.0 + 0.5 * mass_inv[idx] *
                    gamma * dt);
            vec3tr cur_a = vel_invariant_term - mass_inv[idx] * cur_v;
            vec3tr next_r = r[idx] + (cur_v + 0.5 * cur_a * dt) * dt;

            r[idx] = (true_r[idx] = next_r);
            true_prev_v[idx] = cur_v;
            prev_a[idx] = cur_a;
        }

        *t = (*true_t += dt);
        *gen = local_gen;
    }

    void lang_leapfrog_step::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        gamma_factor = vm_inst.find_or_add<real>("gamma_factor",
            params["langevin"]["gamma factor"].as<quantity>());
        temperature = vm_inst.find_or_add<real>("temperature",
            params["langevin"]["temperature"].as<quantity>());
        dt = vm_inst.find_or_add<true_real>("dt",
            params["integrator"]["dt"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        num_particles = vm_inst.find<int>("num_particles");
        mass = vm_inst.find<vector<real>>("mass").to_array();
        gen = &vm_inst.find<rand_gen>("gen");

        F = vm_inst.find_or_emplace<vec3r_vector>("F", num_particles).to_array();
        t = &vm_inst.find_or_emplace<real>("t", (real)0.0);
        true_r = vm_inst.find_or<vec3tr_vector>("true_r", [&]() -> auto& {
            auto& true_r_ = vm_inst.emplace<vec3tr_vector>("true_r", num_particles);
            for (int idx = 0; idx < num_particles; ++idx)
                true_r_[idx] = r[idx];
            return true_r_;
        }).to_array();
        mass_inv = vm_inst.find_or<vector<real>>("mass_inv", [&]() -> auto& {
            auto& mass_inv_ = vm_inst.emplace<vector<real>>("mass_inv", num_particles);
            for (int idx = 0; idx < num_particles; ++idx)
                mass_inv_[idx] = (real)1.0 / mass[idx];
            return mass_inv_;
        }).to_array();
        true_prev_v = vm_inst.find_or_emplace<vec3tr_vector>(
            "true_prev_v", num_particles, vec3tr::Zero()).to_array();
        prev_a = vm_inst.find_or_emplace<vec3tr_vector>(
            "prev_a", num_particles, vec3tr::Zero()).to_array();
        true_t = &vm_inst.find_or_emplace<true_real>("true_t", *t);
    }
}