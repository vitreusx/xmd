#include "nl/divide_into_cells.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <iostream>

namespace xmd::nl {

    void divide_into_cells::operator()() const {
        auto min_val = std::numeric_limits<real>::lowest();
        auto max_val = std::numeric_limits<real>::max();

        auto x_min = max_val, y_min = max_val, z_min = max_val;
        auto x_max = min_val, y_max = min_val, z_max = min_val;

        for (int idx = 0; idx < num_particles; ++idx) {
            auto r_ = box->wrap(r[idx]);
            auto x_ = r_.x(), y_ = r_.y(), z_ = r_.z();
            x_min = min(x_min, x_);
            x_max = max(x_max, x_);
            y_min = min(y_min, y_);
            y_max = max(y_max, y_);
            z_min = min(z_min, z_);
            z_max = max(z_max, z_);
        }

        auto pad = pad_factor * *max_cutoff;
        auto req_r = *max_cutoff + pad;
        auto cell_a = req_r, cell_a_inv = 1.0f / cell_a;

        auto x_ext = x_max - x_min;
        auto cell_nx = (int)ceil(x_ext * cell_a_inv);
        if (cell_nx < 1) cell_nx = 1;
        auto cell_ax = x_ext / (real)cell_nx, cell_ax_inv = (real)1.0 / cell_ax;

        auto y_ext = y_max - y_min;
        auto cell_ny = (int)ceil(y_ext * cell_a_inv);
        if (cell_ny < 1) cell_nx = 1;
        auto cell_ay = y_ext / (real)cell_ny, cell_ay_inv = (real)1.0 / cell_ay;

        auto z_ext = z_max - z_min;
        auto cell_nz = (int)ceil(z_ext * cell_a_inv);
        if (cell_nz < 1) cell_nx = 1;
        auto cell_az = z_ext / (real)cell_nz, cell_az_inv = (real)1.0 / cell_az;

        int num_cells = cell_nx * cell_ny * cell_nz;
        data->num_cells = num_cells;

        data->particles.reinit(num_cells);
        prepare(data->particles);
        for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
            auto r_ = r[part_idx];
            auto ix = adjust<int>(
                floor((r_.x() - x_min) * cell_ax_inv),
                0, cell_nx-1);
            auto iy = adjust<int>(
                floor((r_.y() - y_min) * cell_ay_inv),
                0, cell_ny-1);
            auto iz = adjust<int>(
                floor((r_.z() - z_min) * cell_az_inv),
                0, cell_nz-1);

            auto cell_idx = ix + cell_nx * (iy + cell_ny * iz);
            assign(data->particles, part_idx, cell_idx);
        }
        derive_rest(data->particles);
        data->particles.validate();

//        data->native_contacts.reinit(num_cells);
//        prepare(data->native_contacts);
//
//        for (int nat_idx = 0; nat_idx < nat_cont.size; ++nat_idx) {
//            auto i1 = nat_cont.i1[nat_idx];
//            auto cell_idx1 = data->particles.item_cell_idx[i1];
//            assign(data->native_contacts, 2*nat_idx, cell_idx1);
//
//            auto i2 = nat_cont.i2[nat_idx];
//            auto cell_idx2 = data->particles.item_cell_idx[i2];
//            assign(data->native_contacts, 2*nat_idx+1, cell_idx2);
//        }
//        derive_rest(data->native_contacts);
//        data->native_contacts.validate();

        auto x_scan_r = (int)ceil(req_r / cell_ax);
        auto y_scan_r = (int)ceil(req_r / cell_ay);
        auto z_scan_r = (int)ceil(req_r / cell_az);

        data->neighbor_cells.clear();

        std::vector<std::tuple<int, int, int>> scan_xyz;
        for (int dx = -x_scan_r; dx <= x_scan_r; ++dx) {
            for (int dy = -y_scan_r; dy <= y_scan_r; ++dy) {
                for (int dz = -z_scan_r; dz <= z_scan_r; ++dz) {
                    scan_xyz.emplace_back(dx, dy, dz);
                }
            }
        }

        for (int cell_idx1 = 0; cell_idx1 < num_cells; ++cell_idx1) {
            auto ix1 = cell_idx1 % cell_nx;
            auto rem_xy1 = (cell_idx1 - ix1) / cell_nx;
            auto iy1 = rem_xy1 % cell_ny;
            auto iz1 = rem_xy1 / cell_ny;

            if (data->particles.cell_bucket_size[cell_idx1] == 0)
                continue;

            for (auto const& [dx, dy, dz]: scan_xyz) {
                auto ix2 = (cell_nx + ix1 + dx) % cell_nx;
                auto iy2 = (cell_ny + iy1 + dy) % cell_ny;
                auto iz2 = (cell_nz + iz1 + dz) % cell_nz;
                auto cell_idx2 = ix2 + cell_nx * (iy2 + cell_ny * iz2);

                if (data->particles.cell_bucket_size[cell_idx2] == 0)
                    continue;

                if (cell_idx1 <= cell_idx2) {
                    auto nc_idx = data->neighbor_cells.add();
                    data->neighbor_cells.cell_idx1[nc_idx] = cell_idx1;
                    data->neighbor_cells.cell_idx2[nc_idx] = cell_idx2;
                }
            }
        }

        for (int pair_idx = 0; pair_idx < data->neighbor_cells.size(); ++pair_idx) {
            auto cell_idx1 = data->neighbor_cells.cell_idx1[pair_idx];
            auto cell_idx2 = data->neighbor_cells.cell_idx2[pair_idx];
            if (!(0 <= cell_idx1 && cell_idx1 < num_cells && 0 <= cell_idx2 && cell_idx2 < num_cells))
                throw;
        }

        data->particle_pairs.clear();
        for (int cell_pair_idx = 0; cell_pair_idx < data->neighbor_cells.size(); ++cell_pair_idx) {
            auto cell_idx1 = data->neighbor_cells.cell_idx1[cell_pair_idx];
            auto cell_idx2 = data->neighbor_cells.cell_idx2[cell_pair_idx];

            auto [part_beg1, part_end1] = data->particles.range(cell_idx1);
            auto [part_beg2, part_end2] = data->particles.range(cell_idx2);
//            auto [nc_beg, nc_end] = data->native_contacts.range(cell_idx1);

            for (int bucket_idx1 = part_beg1; bucket_idx1 < part_end1; ++bucket_idx1) {
                auto part_idx1 = data->particles.item_index_buckets[bucket_idx1];
                auto r1 = r[part_idx1];

                for (int bucket_idx2 = part_beg2; bucket_idx2 < part_end2; ++bucket_idx2) {
                    auto part_idx2 = data->particles.item_index_buckets[bucket_idx2];
                    auto r2 = r[part_idx2];

                    if (bucket_idx1 == bucket_idx2 && part_idx1 >= part_idx2)
                        continue;

                    if (part_idx2 - part_idx1 < 3)
                        continue;

                    if (norm(box->r_uv(r1, r2)) < req_r) {
                        auto pair_idx = data->particle_pairs.push_back();
                        if (part_idx1 > 1000000 || part_idx2 > 1000000) {
                            std::cout << part_idx1 << " " << part_idx2 << '\n';
                        }
                        data->particle_pairs.i1[pair_idx] = part_idx1;
                        data->particle_pairs.i2[pair_idx] = part_idx2;

                        bool is_native = false;
//                        for (int nat_bucket_idx = nc_beg; nat_bucket_idx < nc_end; ++nat_bucket_idx) {
//                            auto nat_idx = data->native_contacts.item_index_buckets[nat_bucket_idx];
//                            auto nat_i1 = nat_cont.i1[nat_idx];
//                            auto nat_i2 = nat_cont.i2[nat_idx];
//
//                            if (part_idx1 == nat_i1 && part_idx2 == nat_i2) {
//                                is_native = true;
//                                break;
//                            }
//                        }
                        data->particle_pairs.is_native[pair_idx] = is_native;
                    }
                }
            }
        }

        for (int pair_idx = 0; pair_idx < data->particle_pairs.size; ++pair_idx) {
            auto i1 = data->particle_pairs.i1[pair_idx];
            auto i2 = data->particle_pairs.i2[pair_idx];
            if (!(0 <= i1 && i1 < num_particles && 0 <= i2 && i2 < num_particles))
                throw;
        }

        data->orig_pad = pad;
        for (int idx = 0; idx < num_particles; ++idx)
            data->orig_r[idx] = r[idx];
        data->orig_box = *box;
        data->ref_t = *t;
        *invalid = false;
    }

    void divide_into_cells::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        pad_factor = vm_inst.find_or_emplace<real>("pad_factor",
            params["neighbor list"]["pad factor"].as<quantity>());
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        t = &vm_inst.find<float>("t");

        num_particles = vm_inst.find<int>("num_particles");
        nat_cont = vm_inst.find<native_contact_vector>(
            "native_contacts").to_span();

        data = &vm_inst.find_or<nl_data>("nl_data", [&]() -> auto& {
            auto& data_ = vm_inst.emplace<nl_data>("nl_data");
            data_.particles = spatial_data(num_particles);
            data_.native_contacts = spatial_data(2*nat_cont.size);
            data_.orig_r = vec3r_vector(num_particles);
            return data_;
        });
        max_cutoff = &vm_inst.find_or_emplace<real>("max_cutoff");
        invalid = &vm_inst.find_or_emplace<bool>("invalid", true);
    }

    void divide_into_cells::prepare(spatial_data &sd) const {
        for (int idx = 0; idx < sd.num_cells; ++idx) {
            sd.cell_bucket_size[idx] = 0;
        }
    }

    void
    divide_into_cells::assign(spatial_data &sd, int item_idx, int cell_idx) const {
        sd.item_cell_idx[item_idx] = cell_idx;
        ++sd.cell_bucket_size[cell_idx];
    }

    void divide_into_cells::derive_rest(spatial_data &sd) const {
        int cur_bucket_begin = 0;
        for (int cell_idx = 0; cell_idx < sd.num_cells; ++cell_idx) {
            sd.cell_bucket_begin[cell_idx] = cur_bucket_begin;
            cur_bucket_begin += sd.cell_bucket_size[cell_idx];
        }

        for (int item_idx = 0; item_idx < sd.num_items; ++item_idx) {
            auto cell_idx = sd.item_cell_idx[item_idx];
            sd.item_index_buckets[sd.cell_bucket_begin[cell_idx]++] = item_idx;
        }

        for (int cell_idx = 0; cell_idx < sd.num_cells; ++cell_idx) {
            sd.cell_bucket_begin[cell_idx] -= sd.cell_bucket_size[cell_idx];
        }
    }
}