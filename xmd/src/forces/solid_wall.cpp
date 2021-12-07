#include "forces/solid_wall.h"

namespace xmd {

    void update_solid_wall_pairs::operator()() const {
        pairs->clear();
        for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
            auto r_ = r[part_idx];
            for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                if (abs(signed_dist(r_, walls[wall_idx])) < cutoff) {
                    auto pair_idx = pairs->push_back();
                    pairs->part_idx[pair_idx] = part_idx;
                    pairs->wall_idx[pair_idx] = wall_idx;
                }
            }
        }
    }

    void update_solid_wall_pairs::bind_to_vm(vm &vm_inst) {
        auto& rv = vm_inst.find<vec3r_vector>("r");
        r = rv.to_array();
        num_particles = rv.size();
        walls = vm_inst.find<vector<planef>>("walls").to_span();
        pairs = &vm_inst.find<solid_wall_pair_vector>("wall_pairs");
    }

    void eval_solid_wall_forces::operator()() const {
        for (int idx = 0; idx < pairs.size; ++idx) {
            auto part_idx = pairs.part_idx[idx],
                wall_idx = pairs.wall_idx[idx];

            auto r_ = r[part_idx];
            auto wall = walls[wall_idx];
            auto d = v3::signed_dist(r_, wall);

            auto V_ = eps / (9.0f * ipow<9>(d)), dV_dd = V_ * (-9.0f * d);
            *V += V_;
            F[part_idx] += dV_dd * wall.normal;
            wall_F[wall_idx] -= dV_dd * wall.normal;
        }
    }

    void eval_solid_wall_forces::bind_to_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        walls = vm_inst.find<vector<planef>>("walls").to_span();
        wall_F = vm_inst.find_or_emplace<vec3r_vector>("wall_F",
            walls.size()).to_array();
        pairs = vm_inst.find_or_emplace<solid_wall_pair_vector>(
            "wall_pairs").to_span();
        V = &vm_inst.find<real>("V");
    }

    int solid_wall_pair_vector::push_back()  {
        wall_idx.push_back();
        part_idx.push_back();
        return size++;
    }

    solid_wall_pair_vector::solid_wall_pair_vector(int n):
        wall_idx(n), part_idx(n), size(n) {}

    void solid_wall_pair_vector::clear() {
        wall_idx.clear();
        part_idx.clear();
        size = 0;
    }

    solid_wall_pair_span solid_wall_pair_vector::to_span() {
        solid_wall_pair_span s;
        s.wall_idx = wall_idx.data();
        s.part_idx = part_idx.data();
        s.size = size;
        return s;
    }
}