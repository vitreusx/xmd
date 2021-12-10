#include "nl/divide_into_cells.h"

namespace xmd::nl {

    void divide_into_cells::operator()() const {
        auto min_val = std::numeric_limits<real>::lowest();
        auto max_val = std::numeric_limits<real>::max();

        auto x_min = max_val, y_min = max_val, z_min = max_val;
        auto x_max = min_val, y_max = min_val, z_max = min_val;

        for (int idx = 0; idx < num_particles; ++idx) {
            auto r_ = box->warp_to_box(r[idx]);
            auto x_ = r_.x(), y_ = r_.y(), z_ = r_.z();
            x_min = min(x_min, x_);
            x_max = max(x_max, x_);
            y_min = min(y_min, y_);
            y_max = max(y_max, y_);
            z_min = min(z_min, z_);
            z_max = max(z_max, z_);
        }

        auto req_r = *cutoff + *pad;
        auto cell_a = req_r, cell_a_inv = 1.0f / cell_a;
        auto cell_a_eps = 1e-6;

        auto x_ext = x_max - x_min;
        auto cell_nx = (int)ceil(x_ext * cell_a_inv);
        auto cell_ax = x_ext * (1.0f / (real)cell_nx + cell_a_eps);
        auto cell_ax_inv = 1.0f / cell_ax;

        auto y_ext = y_max - y_min;
        auto cell_ny = (int)ceil(y_ext * cell_a_inv);
        auto cell_ay = y_ext * (1.0f / (real)cell_ny + cell_a_eps);
        auto cell_ay_inv = 1.0f / cell_ay;

        auto z_ext = z_max - z_min;
        auto cell_nz = (int)ceil(z_ext * cell_a_inv);
        auto cell_az = z_ext * (1.0f / (real)cell_nz + cell_a_eps);
        auto cell_az_inv = 1.0f / cell_az;

        int num_cells = cell_nx * cell_ny * cell_nz;
        data->num_cells = num_cells;
        data->cell_num_part.resize(num_cells);
        data->cell_begin.resize(num_cells);

        for (int idx = 0; idx < num_cells; ++idx)
            data->cell_num_part[idx] = 0;

        for (int idx = 0; idx < num_particles; ++idx) {
            auto r_ = r[idx];
            auto ix = (int)ceil((r_.x() - x_min) * cell_ax_inv);
            auto iy = (int)ceil((r_.y() - y_min) * cell_ay_inv);
            auto iz = (int)ceil((r_.z() - z_min) * cell_az_inv);

            auto cell_idx_ = ix + cell_nx * (iy + cell_ny * iz);
            data->cell_idx_for_particle[idx] = cell_idx_;
            ++(data->cell_num_part[cell_idx_]);
        }

        int cur_cell_begin = 0;
        for (int cell_idx = 0; cell_idx < num_cells; ++cell_idx) {
            auto cell_num_part_ = data->cell_num_part[cell_idx];
            data->cell_begin[cell_idx] = cur_cell_begin;
            cur_cell_begin += cell_num_part_;
        }

        for (int idx = 0; idx < num_particles; ++idx) {
            auto cell_idx_ = data->cell_idx_for_particle[idx];
            data->particle_index_groups[data->cell_begin[cell_idx_]++] = idx;
        }

        for (int cell_idx = 0; cell_idx < num_cells; ++cell_idx) {
            data->cell_begin[cell_idx] -= data->cell_num_part[cell_idx];
        }

        auto x_scan_r = (int)ceil(req_r / cell_ax);
        auto y_scan_r = (int)ceil(req_r / cell_ay);
        auto z_scan_r = (int)ceil(req_r / cell_az);

        data->neighbor_cells.clear();
        for (int ix = 0; ix < cell_nx; ++ix) {
            for (int dx = -x_scan_r; dx <= x_scan_r; ++dx) {
                auto ix2 = (cell_nx + ix + dx) % cell_nx;
                for (int iy = 0; iy < cell_ny; ++iy) {
                    for (int dy = -y_scan_r; dy <= y_scan_r; ++dy) {
                        auto iy2 = (cell_ny + iy + dy) % cell_ny;
                        for (int iz = 0; iz < cell_nz; ++iz) {
                            auto cell_idx1 = ix + cell_nx * (iy + cell_ny * iz);
                            for (int dz = -z_scan_r; dz <= z_scan_r; ++dz) {
                                auto iz2 = (cell_nz + iz + dz) % cell_nz;
                                auto cell_idx2 = ix2 + cell_nx * (iy2 + cell_ny * iz2);

                                if (cell_idx1 <= cell_idx2) {
                                    auto nc_idx = data->neighbor_cells.add();
                                    data->neighbor_cells.cell_idx1[nc_idx] = cell_idx1;
                                    data->neighbor_cells.cell_idx2[nc_idx] = cell_idx2;
                                }
                            }
                        }
                    }
                }
            }
        }

        data->pad = *pad;
        for (int idx = 0; idx < num_particles; ++idx)
            orig_r[idx] = r[idx];
        *orig_box = *box;
        *invalid = false;
    }

    void divide_into_cells::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        num_particles = vm_inst.find<int>("num_particles");
        data = &vm_inst.find_or<nl_data>("nl_data", [&]() -> auto& {
            auto& data_ = vm_inst.emplace<nl_data>("nl_data");
            data_.cell_idx_for_particle.resize(num_particles);
            data_.particle_index_groups.resize(num_particles);
            return data_;
        });
        cutoff = &vm_inst.find_or_emplace<real>("cutoff");
        pad = &vm_inst.find_or_emplace<real>("pad");
        invalid = &vm_inst.find_or_emplace<bool>("invalid", true);

        orig_r = vm_inst.find_or_emplace<vec3r_vector>("orig_r",
            num_particles).to_array();
        orig_box = &vm_inst.find_or_emplace<xmd::box<vec3r>>("orig_box");
    }
}