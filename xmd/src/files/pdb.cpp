#include "pdb.h"
#include "detail/fields.h"
#include "detail/records.h"
#include <sstream>
#include <string_view>

namespace xmd {
    pdb::pdb(std::istream &is) {
        std::unordered_map<char, bool> ter_found;

        size_t cur_model = 1;
        models.push_back(cur_model);

        auto find_or_add_chain = [&](char chain_id) -> auto {
            auto chain_iter = chains.find(chain_id);
            if (chain_iter == chains.end()) {
                chain c;
                c.chain_id = chain_id;

                chains[chain_id] = c;
                chain_iter = chains.find(chain_id);
            }

            return &chain_iter->second;
        };

        auto find_or_add_res = [&](chain &c, size_t seq_num,
            const std::string &name) -> auto {

            auto res_iter = c.residues.find(seq_num);
            if (res_iter == c.residues.end()) {
                residue r;
                r.seq_num = seq_num;
                r.name = name;
                r.parent_chain = &c;

                c.residues[seq_num] = r;
                res_iter = c.residues.find(seq_num);
                if (!ter_found[c.chain_id])
                    c.order.push_back(&res_iter->second);
            }

            return &res_iter->second;
        };

        for (std::string line; std::getline(is, line); ) {
            if (auto opt_record = records::record::try_parse(line); opt_record) {
                auto& record_r = opt_record.value();

                if (auto atom_r = record_r.cast<records::atom>(); atom_r) {
                    auto *chain = find_or_add_chain(atom_r->chain_id);

                    auto *res = find_or_add_res(*chain,
                        atom_r->res_seq_num, atom_r->residue_name);

                    auto *atm = &chain->atoms[atom_r->serial];

                    atm->name = atom_r->atom_name;
                    atm->serial = atom_r->serial;
                    atm->pos_for_model[cur_model] = atom_r->pos;
                    atm->parent_res = res;

                    res->atoms.push_back(atm);
                }
                else if (auto ssbond_r = record_r.cast<records::ssbond>(); ssbond_r) {
                    disulfide_bond ss;
                    ss.serial = ssbond_r->serial;
                    ss.length = ssbond_r->length;

                    auto& chain1 = *find_chain(ssbond_r->res[0].chain_id);
                    auto& res1 = *find_res(chain1, ssbond_r->res[0].res_seq_num);
                    ss.a1 = res1.find_by_name("SG");

                    auto& chain2 = *find_chain(ssbond_r->res[1].chain_id);
                    auto& res2 = *find_res(chain2, ssbond_r->res[1].res_seq_num);
                    ss.a2 = res2.find_by_name("SG");

                    disulfide_bonds[ss.serial] = ss;
                }
                else if (auto link_r = record_r.cast<records::link>(); link_r) {
                    link _link;
                    _link.length = link_r->length;

                    auto& chain1 = *find_chain(link_r->res[0].chain_id);
                    auto& res1 = *find_res(chain1, link_r->res[0].res_seq_num);
                    _link.a1 = res1.find_by_name(link_r->res[0].atom_name);

                    auto& chain2 = *find_chain(link_r->res[1].chain_id);
                    auto& res2 = *find_res(chain2, link_r->res[1].res_seq_num);
                    _link.a2 = res2.find_by_name(link_r->res[1].atom_name);

                    links.push_back(_link);
                }
                else if (auto cryst1_r = record_r.cast<records::cryst1>(); cryst1_r) {
                    cryst1 = cryst1_r->cell;
                }
                else if (auto ter_r = record_r.cast<records::ter>(); ter_r) {
                    ter_found[ter_r->chain_id] = true;
                    auto& c = *find_chain(ter_r->chain_id);
                    c.ter_serial = ter_r->serial;
                }
                else if (auto model_r = record_r.cast<records::model>(); model_r) {
                    cur_model = model_r->serial;
                }
                else if (auto end_r = record_r.cast<records::end>(); end_r) {
                    return;
                }
            }
        }
    }

    pdb::atom *pdb::residue::find_by_name(const std::string &name) {
        for (auto *atm: atoms) {
            if (atm->name == name)
                return atm;
        }
        return nullptr;
    }

    pdb::chain *pdb::find_chain(char chain_id) {
        auto chain_iter = chains.find(chain_id);
        if (chain_iter != chains.end())
            return &chain_iter->second;
        else
            return nullptr;
    }

    pdb::residue *pdb::find_res(pdb::chain &c, size_t seq_num) {
        auto res_iter = c.residues.find(seq_num);
        if (res_iter != c.residues.end())
            return &res_iter->second;
        else
            return nullptr;
    }

    std::ostream &operator<<(std::ostream &os, const pdb &p) {
        for (auto model: p.models) {
            records::model model_r;
            model_r.serial = model;
            os << model_r.write();

            for (auto const& [chain_id, chain]: p.chains) {
                for (auto const& res: chain.order) {
                    for (auto const& atm: res->atoms) {
                        records::atom atom_r;
                        atom_r.serial = atm->serial;
                        atom_r.chain_id = chain.chain_id;
                        atom_r.res_seq_num = res->seq_num;
                        atom_r.residue_name = res->name;
                        atom_r.atom_name = atm->name;
                        atom_r.pos = atm->pos_for_model[model];

                        os << '\n' << atom_r.write();
                    }
                }

                auto final_res = chain.order.back();
                records::ter ter_r;
                ter_r.chain_id = chain.chain_id;
                ter_r.res_seq_num = final_res->seq_num;
                ter_r.serial = chain.ter_serial;
                ter_r.res_name = final_res->name;

                os << '\n' << ter_r.write();
            }

            os << '\n' << records::endmdl().write();
        }

        for (auto const& [serial, ss]: p.disulfide_bonds) {
            records::ssbond ssbond_r;
            ssbond_r.serial = ss.serial;
            ssbond_r.length = ss.length;

            ssbond_r.res[0].res_seq_num = ss.a1->parent_res->seq_num;
            ssbond_r.res[0].chain_id = ss.a1->parent_res->parent_chain->chain_id;
            ssbond_r.res[1].res_seq_num = ss.a2->parent_res->seq_num;
            ssbond_r.res[1].chain_id = ss.a2->parent_res->parent_chain->chain_id;

            os << '\n' << ssbond_r.write();
        }

        for (auto const& _link: p.links) {
            records::link link_r;
            link_r.length = _link.length;

            link_r.res[0].chain_id = _link.a1->parent_res->parent_chain->chain_id;
            link_r.res[0].res_seq_num = _link.a1->parent_res->seq_num;
            link_r.res[0].atom_name = _link.a1->name;
            link_r.res[0].res_name = _link.a1->parent_res->name;

            link_r.res[1].chain_id = _link.a2->parent_res->parent_chain->chain_id;
            link_r.res[1].res_seq_num = _link.a2->parent_res->seq_num;
            link_r.res[1].atom_name = _link.a2->name;
            link_r.res[1].res_name = _link.a2->parent_res->name;

            os << '\n' << link_r.write();
        }

        return os;
    }
}