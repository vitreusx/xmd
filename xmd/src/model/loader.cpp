#include "model/loader.h"
#include <xmd/types/vec3.h>
#include <xmd/model/native_contact.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <fstream>
#include <xmd/utils/units.h>
#include <xmd/utils/convert.h>
#include <algorithm>

namespace xmd {

    void model_loader::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");

        auto& stored_model = ctx.ephemeral<model>("model");
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
            auto& gen = ctx.var<rand_gen>("gen");

            std::optional<true_real> bond_dist;
            if (auto bond_node = saw_node["bond distance"]; bond_node) {
                auto bond_dist_str = bond_node.as<std::string>();
                if (!bond_dist_str.empty())
                    bond_dist = quantity(bond_dist_str);
            }

            auto res_dens = saw_node["residue density"].as<quantity>();
            auto infer_box = saw_node["infer box"].as<bool>();

            stored_model.morph_into_saw(gen, bond_dist, res_dens, infer_box);
        }

        auto& num_particles = ctx.persistent<int>(
            "num_particles", stored_model.residues.size());

        auto& res_map = ctx.ephemeral<res_map_t>("res_map");
        int res_idx = 0;
        for (auto const& res: stored_model.residues) {
            res_map[res.get()] = res_idx++;
        }

        ctx.persistent<vector<vec3r>>("r", lazy([&]() -> auto {
            vector<vec3r> r(num_particles);

            for (auto const& res: stored_model.residues) {
                r[res_map[res.get()]] = res->pos;
            }

            return r;
        }));

        ctx.persistent<vector<amino_acid>>("atype", lazy([&]() -> auto {
            vector<amino_acid> atype(num_particles);

            for (auto const& res: stored_model.residues) {
                atype[res_map[res.get()]] = res->type;
            }

            return atype;
        }));

        ctx.persistent<vector<real>>("mass", num_particles, (real)1.0);

        ctx.persistent<xmd::box>("box", lazy([&]() -> auto {
            xmd::box box;
            box.cell = stored_model.model_box.cell;
            box.cell_inv = stored_model.model_box.cell_inv;
            return box;
        }));

        ctx.persistent<vector<nat_cont>>("native_contacts", lazy([&]() -> auto {
            vector<nat_cont> nc;

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
        }));

        ctx.persistent<vector<int>>("prev", lazy([&]() -> auto {
            vector<int> prev(num_particles, -1);

            for (auto const& res: stored_model.residues) {
                if (res->seq_idx > 0) {
                    auto prev_ptr = res->parent->residues[res->seq_idx-1];
                    prev[res_map[res.get()]] = res_map[prev_ptr];
                }
            }
            return prev;
        }));

        ctx.persistent<vector<int>>("next", lazy([&]() -> auto {
            vector<int> next(num_particles, -1);
            for (auto const& res: stored_model.residues) {
                if (res->seq_idx < (int)res->parent->residues.size()-1) {
                    auto next_ptr = res->parent->residues[res->seq_idx+1];
                    next[res_map[res.get()]] = res_map[next_ptr];
                }
            }
            return next;
        }));

        ctx.persistent<vector<int>>("chain_idx", lazy([&]() -> auto {
            vector<int> chain_idx(num_particles, -1);
            for (auto const& res: stored_model.residues) {
                chain_idx[res_map[res.get()]] = res->parent->chain_idx;
            }
            return chain_idx;
        }));

        ctx.persistent<vector<int>>("seq_idx", lazy([&]() -> auto {
            vector<int> seq_idx(num_particles, -1);
            for (auto const& res: stored_model.residues) {
                seq_idx[res_map[res.get()]] = res->seq_idx;
            }
            return seq_idx;
        }));
    }
}