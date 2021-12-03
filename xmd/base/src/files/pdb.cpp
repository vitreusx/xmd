#include "files/pdb.h"
#include "fields.h"
#include "records.h"
#include <sstream>
#include <string_view>
#include <map>
#include "utils/units.h"

namespace xmd {
    pdb::pdb(std::istream &is) {
        std::unordered_map<char, bool> ter_found;

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
                    atm->pos = atom_r->pos * angstrom;
                    atm->parent_res = res;

                    res->atoms.push_back(atm);
                }
                else if (auto ssbond_r = record_r.cast<records::ssbond>(); ssbond_r) {
                    disulfide_bond ss;
                    ss.serial = ssbond_r->serial;
                    ss.length = ssbond_r->length * angstrom;

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
                    _link.length = link_r->length * angstrom;

                    auto& chain1 = *find_chain(link_r->res[0].chain_id);
                    auto& res1 = *find_res(chain1, link_r->res[0].res_seq_num);
                    _link.a1 = res1.find_by_name(link_r->res[0].atom_name);

                    auto& chain2 = *find_chain(link_r->res[1].chain_id);
                    auto& res2 = *find_res(chain2, link_r->res[1].res_seq_num);
                    _link.a2 = res2.find_by_name(link_r->res[1].atom_name);

                    links.push_back(_link);
                }
                else if (auto cryst1_r = record_r.cast<records::cryst1>(); cryst1_r) {
                    cryst1 = cryst1_r->cell * angstrom;
                }
                else if (auto ter_r = record_r.cast<records::ter>(); ter_r) {
                    ter_found[ter_r->chain_id] = true;
                    auto& c = *find_chain(ter_r->chain_id);
                    c.ter_serial = ter_r->serial;
                }
                else if (auto end_r = record_r.cast<records::end>(); end_r) {
                    return;
                }
            }
        }
    }

    pdb::pdb(const xmd::model &xmd_model) {
        char chain_id = 'A';
        size_t res_seq_num = 1, atom_serial = 1;

        std::unordered_map<xmd::model::residue const*, residue*> res_map;

        for (auto const& xmd_chain: xmd_model.chains) {
            auto& pdb_chain = chains[chain_id];
            pdb_chain.chain_id = chain_id;

            for (auto const& xmd_res_ref: xmd_chain->residues) {
                auto& xmd_res = *xmd_res_ref;

                auto& ca_atom = pdb_chain.atoms[atom_serial];
                ca_atom.name = "CA";
                ca_atom.serial = atom_serial;
                ca_atom.pos = xmd_res.pos;

                auto& pdb_res = pdb_chain.residues[res_seq_num];
                pdb_res.parent_chain = &pdb_chain;
                pdb_res.seq_num = res_seq_num;
                pdb_res.name = xmd_res.type.name();

                ca_atom.parent_res = &pdb_res;
                pdb_res.atoms.push_back(&ca_atom);
                pdb_chain.order.push_back(&pdb_res);
                res_map[&xmd_res] = &pdb_res;

                ++res_seq_num;
                ++atom_serial;
            }

            ++chain_id;
        }

        for (auto const& xmd_cont: xmd_model.contacts) {
            auto *pdb_res1 = res_map[&*xmd_cont.res1];
            auto *pdb_res2 = res_map[&*xmd_cont.res2];

            link pdb_cont;
            pdb_cont.a1 = pdb_res1->atoms[0];
            pdb_cont.a2 = pdb_res2->atoms[0];
            pdb_cont.length = xmd_cont.length;

            links.push_back(pdb_cont);
        }

        size_t ss_serial = 1;
        for (auto const& xmd_ss: xmd_model.disulfide_bonds) {
            auto *pdb_res1 = res_map[&*xmd_ss.res1];
            auto *pdb_res2 = res_map[&*xmd_ss.res2];

            disulfide_bond pdb_ss;
            pdb_ss.a1 = pdb_res1->atoms[0];
            pdb_ss.a2 = pdb_res2->atoms[0];
            pdb_ss.serial = ss_serial;
            pdb_ss.length = xmd_ss.length;

            disulfide_bonds[ss_serial] = pdb_ss;
            ++ss_serial;
        }

        cryst1 = xmd_model.model_box.cell;
    }

    pdb::atom *pdb::residue::find_by_name(const std::string &name) const {
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
        os << p.emit_model(1);
        os << p.emit_contacts();
        return os;
    }

    xmd::model pdb::to_model() const {
        xmd::model xmd_model;

        std::unordered_map<residue const*, xmd::model::residue*> res_map;

        int chain_idx = 0;
        for (auto const& [chain_id, pdb_chain]: chains) {
            auto& xmd_chain = xmd_model.chains.emplace_back(
                std::make_unique<xmd::model::chain>());
            xmd_chain->chain_idx = chain_idx++;

            int res_seq_idx = 0;
            for (auto const& [res_seq_num, pdb_res]: pdb_chain.residues) {
                auto& xmd_res = xmd_model.residues.emplace_back(
                    std::make_unique<xmd::model::residue>());

                xmd_res->type = amino_acid(pdb_res.name);
                xmd_res->pos = pdb_res.find_by_name("CA")->pos;
                xmd_res->parent = xmd_chain.get();
                xmd_res->seq_idx = res_seq_idx++;

                xmd_chain->residues.push_back(&*xmd_res);
                res_map[&pdb_res] = &*xmd_res;
            }

            for (size_t res_idx = 0; res_idx + 2 < xmd_chain->residues.size(); ++res_idx) {
                auto res1 = xmd_chain->residues[res_idx];
                auto res2 = xmd_chain->residues[res_idx+1];
                auto res3 = xmd_chain->residues[res_idx+2];

                auto r1 = res1->pos, r2 = res2->pos, r3 = res3->pos;
                auto r12_u = (r2 - r1).normalized(), r23_u = (r3 - r2).normalized();
                auto theta = acos(r12_u.dot(r23_u));

                xmd::model::angle xmd_angle;
                xmd_angle.res1 = res1;
                xmd_angle.res2 = res2;
                xmd_angle.res3 = res3;
                xmd_angle.theta = theta;

                xmd_model.angles.push_back(xmd_angle);
            }

            for (size_t res_idx = 0; res_idx + 3 < xmd_chain->residues.size(); ++res_idx) {
                auto res1 = xmd_chain->residues[res_idx];
                auto res2 = xmd_chain->residues[res_idx+1];
                auto res3 = xmd_chain->residues[res_idx+2];
                auto res4 = xmd_chain->residues[res_idx+3];

                auto r1 = res1->pos, r2 = res2->pos, r3 = res3->pos,
                     r4 = res4->pos;
                auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
                auto x12_23 = r12.cross(r23), x23_34 = r23.cross(r34);

                auto x12_23_u = x12_23.normalized(), x23_34_u = x23_34.normalized();

                auto cos_phi = x12_23_u.dot(x23_34_u);
                auto phi = acos(cos_phi);
                if (x12_23.dot(r34) < 0.0f) phi = -phi;

                xmd::model::dihedral xmd_dihedral;
                xmd_dihedral.res1 = res1;
                xmd_dihedral.res2 = res2;
                xmd_dihedral.res3 = res3;
                xmd_dihedral.res4 = res4;
                xmd_dihedral.phi = phi;

                xmd_model.dihedrals.push_back(xmd_dihedral);
            }
        }

        std::map<std::pair<residue const*, residue const*>, double> contacts_map;
        for (auto const& pdb_link: links) {
            auto *res1 = pdb_link.a1->parent_res;
            auto *res2 = pdb_link.a2->parent_res;
            if (res1 >= res2) std::swap(res1, res2);

            contacts_map[{res1, res2}] = pdb_link.length;
        }
        for (auto const& [res1_res2, length]: contacts_map) {
            xmd::model::contact xmd_cont;

            auto const& [res1, res2] = res1_res2;
            xmd_cont.res1 = res_map[res1];
            xmd_cont.res2 = res_map[res2];
            xmd_cont.length = length;

            xmd_model.contacts.push_back(xmd_cont);
        }

        std::map<std::pair<residue const*, residue const*>, double> ss_map;
        for (auto const& [ss_serial, pdb_ss]: disulfide_bonds) {
            auto *res1 = pdb_ss.a1->parent_res;
            auto *res2 = pdb_ss.a2->parent_res;
            if (res1 >= res2) std::swap(res1, res2);

            ss_map[{res1, res2}] = pdb_ss.length;
        }
        for (auto const& [res1_res2, length]: ss_map) {
            xmd::model::contact xmd_ss;

            auto const& [res1, res2] = res1_res2;
            xmd_ss.res1 = res_map[res1];
            xmd_ss.res2 = res_map[res2];
            xmd_ss.length = length;

            xmd_model.disulfide_bonds.push_back(xmd_ss);
        }

        xmd_model.model_box.set_cell(cryst1);

        return xmd_model;
    }

    void pdb::add_contacts(amino_acid_data const& data,
        bool all_atoms) {

        std::map<std::pair<atom*, atom*>, double> contact_map;
        for (auto const& pdb_link: links) {
            contact_map[{pdb_link.a1, pdb_link.a2}] = pdb_link.length;
        }
        for (auto const& [ss_serial, pdb_ss]: disulfide_bonds) {
            contact_map[{pdb_ss.a1, pdb_ss.a2}] = pdb_ss.length;
        }

        for (auto& [chain1_id, chain1]: chains) {
            for (auto& [atom1_serial, atom1]: chain1.atoms) {
                if (!all_atoms && atom1.name != "CA")
                    continue;

                auto res1 = amino_acid(atom1.parent_res->name);
                auto radius1 = all_atoms
                    ? data[res1].atoms.at(atom1.name).radius
                    : data[res1].radius;
                auto seq1 = (int)atom1.parent_res->seq_num;

                for (auto& [chain2_id, chain2]: chains) {
                    for (auto& [atom2_serial, atom2]: chain2.atoms) {
                        if (!all_atoms && atom2.name != "CA")
                            continue;
                        if (atom1.parent_res == atom2.parent_res)
                            continue;

                        auto res2 = amino_acid(atom2.parent_res->name);
                        auto radius2 = all_atoms
                            ? data[res2].atoms.at(atom2.name).radius
                            : data[res2].radius;
                        auto seq2 = (int)atom2.parent_res->seq_num;

                        if (abs(seq1 - seq2) < 3)
                            continue;

                        auto dist = (atom1.pos - atom2.pos).norm();
                        auto max_overlap_dist = radius1 + radius2;
                        if (dist < max_overlap_dist) {
                            contact_map[{&atom1, &atom2}] = dist;
                        }
                    }
                }
            }
        }

        std::map<std::pair<atom*, atom*>, double> ss_bonds;

        links = {};
        disulfide_bonds = {};
        size_t ss_serial = 1;

        for (auto const& [atom1_atom2, dist]: contact_map) {
            auto const& [atom1, atom2] = atom1_atom2;
            if (atom1->parent_res->name == "CYS" &&
                atom2->parent_res->name == "CYS") {

                disulfide_bond ss;
                ss.serial = ss_serial;
                ss.a1 = atom1;
                ss.a2 = atom2;
                ss.length = dist;

                disulfide_bonds[ss_serial] = ss;
                ++ss_serial;
            }
            else {
                link pdb_link;
                pdb_link.a1 = atom1;
                pdb_link.a2 = atom2;
                pdb_link.length = dist;

                links.push_back(pdb_link);
            }
        }
    }

    pdb::pdb(const pdb &other) {
        *this = other;
    }

    pdb &pdb::operator=(const pdb &other) {
        std::unordered_map<atom const*, atom*> atom_map;
        std::unordered_map<residue const*, residue*> res_map;
        std::unordered_map<chain const*, chain*> chain_map;

        chains = {};
        for (auto const& [chain_idx, other_chain]: other.chains) {
            auto& xmd_chain = chains[chain_idx];
            chain_map[&other_chain] = &xmd_chain;
            xmd_chain = other_chain;

            xmd_chain.atoms = {};
            for (auto const& [atom_ser, other_atom]: other_chain.atoms) {
                auto& xmd_atom = xmd_chain.atoms[atom_ser];
                atom_map[&other_atom] = &xmd_atom;
                xmd_atom = other_atom;
            }

            xmd_chain.residues = {};
            for (auto const& [seq_num, other_res]: other_chain.residues) {
                auto& xmd_res = xmd_chain.residues[seq_num];
                res_map[&other_res] = &xmd_res;
                xmd_res = other_res;
            }

            for (auto& [atom_ser, atom]: xmd_chain.atoms) {
                atom.parent_res = res_map[atom.parent_res];
            }

            for (auto& [seq_num, res]: xmd_chain.residues) {
                res.parent_chain = chain_map[res.parent_chain];
                for (auto& atom_ptr: res.atoms) {
                    atom_ptr = atom_map[atom_ptr];
                }
            }

            for (auto& res_ptr: xmd_chain.order) {
                res_ptr = res_map[res_ptr];
            }
        }

        disulfide_bonds = other.disulfide_bonds;
        for (auto& [serial, ss]: disulfide_bonds) {
            ss.a1 = atom_map[ss.a1];
            ss.a2 = atom_map[ss.a2];
        }

        links = other.links;
        for (auto& xmd_link: links) {
            xmd_link.a1 = atom_map[xmd_link.a1];
            xmd_link.a2 = atom_map[xmd_link.a2];
        }

        cryst1 = other.cryst1;

        return *this;
    }

    pdb_model_emitter pdb::emit_model(int model_serial) const {
        return pdb_model_emitter(*this, model_serial);
    }

    pdb_contacts_emitter pdb::emit_contacts() const {
        return pdb_contacts_emitter(*this);
    }

    std::ostream &
    operator<<(std::ostream &os, const pdb_model_emitter &emitter) {
        records::model model_r;
        model_r.serial = emitter.model_serial;
        os << model_r.write();

        for (auto const&[chain_id, chain]: emitter.owner.chains) {
            for (auto const &res: chain.order) {
                for (auto const &atm: res->atoms) {
                    records::atom atom_r;
                    atom_r.serial = atm->serial;
                    atom_r.chain_id = chain.chain_id;
                    atom_r.res_seq_num = res->seq_num;
                    atom_r.residue_name = res->name;
                    atom_r.atom_name = atm->name;
                    atom_r.pos = atm->pos / angstrom;

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

        return os;
    }

    pdb_model_emitter::pdb_model_emitter(const pdb &owner, int model_serial):
        owner{owner}, model_serial{model_serial} {};

    std::ostream &
    operator<<(std::ostream &os, const pdb_contacts_emitter &emitter) {
        for (auto const& [serial, ss]: emitter.owner.disulfide_bonds) {
            records::ssbond ssbond_r;
            ssbond_r.serial = ss.serial;
            ssbond_r.length = ss.length / angstrom;

            ssbond_r.res[0].res_seq_num = ss.a1->parent_res->seq_num;
            ssbond_r.res[0].chain_id = ss.a1->parent_res->parent_chain->chain_id;
            ssbond_r.res[1].res_seq_num = ss.a2->parent_res->seq_num;
            ssbond_r.res[1].chain_id = ss.a2->parent_res->parent_chain->chain_id;

            os << '\n' << ssbond_r.write();
        }

        for (auto const& _link: emitter.owner.links) {
            records::link link_r;
            link_r.length = _link.length / angstrom;

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

    pdb_contacts_emitter::pdb_contacts_emitter(const pdb &owner):
        owner{owner} {};
}