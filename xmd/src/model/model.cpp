#include "model.h"
#include <unordered_map>

namespace xmd {
    model::model(const model &other) {
        *this = other;
    }

    model &model::operator=(const model &other) {
        std::unordered_map<residue const*, residue_ref> res_map;

        residues = {};
        for (auto const& other_res: other.residues) {
            auto res_ref = residues.emplace(residues.end(), other_res);
            res_map[&other_res] = res_ref;
        }

        chains = other.chains;
        for (auto& _chain: chains) {
            for (auto& res_ref: _chain.residues) {
                res_ref = res_map[&*res_ref];
            }
        }

        contacts = other.contacts;
        for (auto& cont: contacts) {
            cont.res1 = res_map[&*cont.res1];
            cont.res2 = res_map[&*cont.res2];
        }

        angles = other.angles;
        for (auto& _angle: angles) {
            _angle.res1 = res_map[&*_angle.res1];
            _angle.res2 = res_map[&*_angle.res2];
            _angle.res3 = res_map[&*_angle.res3];
        }

        dihedrals = other.dihedrals;
        for (auto& _dihedral: dihedrals) {
            _dihedral.res1 = res_map[&*_dihedral.res1];
            _dihedral.res2 = res_map[&*_dihedral.res2];
            _dihedral.res3 = res_map[&*_dihedral.res3];
            _dihedral.res4 = res_map[&*_dihedral.res4];
        }

        cell = other.cell;

        return *this;
    }

    model &model::operator+=(const model &m2) {
        auto m2_copy = m2;

        residues.splice(residues.end(), std::move(m2_copy.residues));
        chains.splice(chains.end(), std::move(m2_copy.chains));
        contacts.splice(contacts.end(), std::move(m2_copy.contacts));
        angles.splice(angles.end(), std::move(m2_copy.angles));
        dihedrals.splice(dihedrals.end(), std::move(m2_copy.dihedrals));

        return *this;
    }

    model operator+(const model &m1, const model &m2) {
        model sum;
        sum += m1;
        sum += m2;
        return sum;
    }
}