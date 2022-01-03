#include "model/loader.h"
#include <xmd/types/vec3.h>
#include <xmd/model/native_contact.h>
#include <xmd/params/param_file.h>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <fstream>
#include <xmd/utils/units.h>
#include <xmd/utils/convert.h>
#include <algorithm>

namespace xmd {

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

        vm_inst.find_or<vector<vec3r>>("r", [&]() -> auto& {
            auto& r = vm_inst.emplace<vector<vec3r>>("r", num_particles);
            int res_idx = 0;
            for (auto const& res: stored_model.residues) {
                res_map[res.get()] = res_idx;
                r[res_idx] = convert<real, double>(res->pos);
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
            box.cell = convert<real>(stored_model.model_box.cell);
            box.cell_inv = convert<real>(stored_model.model_box.cell_inv);
            return box;
        });

        vm_inst.find_or<vector<nat_cont>>("native_contacts", [&]() -> auto& {
            auto& nc = vm_inst.emplace<vector<nat_cont>>("native_contacts");

            for (int nc_idx = 0; nc_idx < (int)stored_model.contacts.size(); ++nc_idx) {
                auto const& xmd_cont = stored_model.contacts[nc_idx];
                auto i1 = res_map[xmd_cont.res1], i2 = res_map[xmd_cont.res2];
                if (i1 >= i2) std::swap(i1, i2);
                nc.emplace_back(i1, i2, (real)xmd_cont.length);
            }

            std::sort(nc.begin(), nc.end(), [](auto&& p, auto&& q) -> bool {
                return p.i1() < q.i1() || (p.i1() == q.i1() && p.i2() < q.i2());
            });

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

        vm_inst.find_or<vector<int>>("chain_idx", [&]() -> auto& {
            auto& chain_idx = vm_inst.emplace<vector<int>>("chain_idx",
                num_particles, -1);
            for (auto const& res: stored_model.residues) {
                chain_idx[res_map[res.get()]] = res->parent->chain_idx;
            }
            return chain_idx;
        });

        vm_inst.find_or<vector<int>>("seq_idx", [&]() -> auto& {
            auto& seq_idx = vm_inst.emplace<vector<int>>("seq_idx",
                num_particles, -1);
            for (auto const& res: stored_model.residues) {
                seq_idx[res_map[res.get()]] = res->seq_idx;
            }
            return seq_idx;
        });
    }
}