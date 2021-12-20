#include "model/loader.h"
#include <xmd/types/vec3.h>
#include <xmd/model/native_contacts.h>
#include <xmd/params/param_file.h>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <fstream>
#include <xmd/utils/units.h>

namespace xmd {
    static vec3r cast_v(Eigen::Vector3d const& v) {
        return { (real)v.x(), (real)v.y(), (real)v.z() };
    }

    void model_loader::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");

        auto& stored_model = vm_inst.find_or_emplace<model>("model");
        if (auto seqfile_node = params["model"]["seqfile"]; seqfile_node) {
            auto seqfile = seq_file(seqfile_node.as<std::string>());
            stored_model = seqfile.to_model();
        }
        else if (auto pdbfile_node = params["model"]["pdb"]; pdbfile_node) {
            auto stream = std::ifstream(pdbfile_node.as<std::string>());
            auto pdbfile = pdb(std::move(stream));
            stored_model = pdbfile.to_model();
        }

        if (auto saw_node = params["model"]["morph into SAW"]; saw_node) {
            auto& seed = vm_inst.find<int>("seed");
            std::default_random_engine eng(seed);

            std::optional<true_real> bond_dist;
            if (auto bond_node = saw_node["bond distance"]; bond_node) {
                auto bond_dist_str = bond_node.as<std::string>();
                if (!bond_dist_str.empty())
                    bond_dist = quantity(bond_dist_str);
            }

            auto res_dens = saw_node["residue density"].as<quantity>();
            auto infer_box = saw_node["infer box"].as<bool>();

            stored_model.morph_into_saw(eng, bond_dist, res_dens, infer_box);
        }

        auto& num_particles = vm_inst.find_or_emplace<int>(
            "num_particles", stored_model.residues.size());

        auto& res_map = vm_inst.find_or_emplace<res_map_t>("res_map");

        vm_inst.find_or<vec3r_vector>("r", [&]() -> auto& {
            auto& r = vm_inst.emplace<vec3r_vector>("r", num_particles);
            int res_idx = 0;
            for (auto const& res: stored_model.residues) {
                res_map[res.get()] = res_idx;
                r[res_idx] = cast_v(res->pos);
                res_idx++;
            }
            return r;
        });

        vm_inst.find_or<vector<amino_acid>>("atype", [&]() -> auto& {
            auto& atype = vm_inst.emplace<vector<amino_acid>>("atype", num_particles);
            for (auto const& res: stored_model.residues) {
                atype[res_map[res.get()]] = res->type;
            }

            return atype;
        });

        vm_inst.find_or_emplace<vector<real>>("mass", num_particles, (real)1.0);

        vm_inst.find_or<xmd::box<vec3r>>("box", [&]() -> auto& {
            auto& box = vm_inst.emplace<xmd::box<vec3r>>("box");
            box.cell = cast_v(stored_model.model_box.cell);
            box.cell_inv = cast_v(stored_model.model_box.cell_inv);
            return box;
        });

        vm_inst.find_or<native_contact_vector>("native_contacts", [&]() -> auto& {
            auto& nc = vm_inst.emplace<native_contact_vector>(
                "native_contacts", stored_model.contacts.size());

            for (int nc_idx = 0; nc_idx < nc.size; ++nc_idx) {
                auto const& xmd_cont = stored_model.contacts[nc_idx];
                auto i1 = res_map[xmd_cont.res1], i2 = res_map[xmd_cont.res2];
                if (i1 >= i2) std::swap(i1, i2);
                nc.i1[nc_idx] = i1;
                nc.i2[nc_idx] = i2;
            }

            nc.sort();
            return nc;
        });

        vm_inst.find_or<vector<int>>("prev", [&]() -> auto& {
            auto& prev = vm_inst.emplace<vector<int>>("prev",
                num_particles, -1);
            for (auto const& res: stored_model.residues) {
                if (res->seq_idx > 0) {
                    auto prev_ptr = res->parent->residues[res->seq_idx-1];
                    prev[res_map[res.get()]] = res_map[prev_ptr];
                }
            }
            return prev;
        });

        vm_inst.find_or<vector<int>>("next", [&]() -> auto& {
            auto& next = vm_inst.emplace<vector<int>>("next",
                num_particles, -1);
            for (auto const& res: stored_model.residues) {
                if (res->seq_idx < (int)res->parent->residues.size()-1) {
                    auto next_ptr = res->parent->residues[res->seq_idx+1];
                    next[res_map[res.get()]] = res_map[next_ptr];
                }
            }
            return next;
        });
    }
}