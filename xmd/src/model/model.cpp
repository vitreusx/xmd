#include "model/model.h"
#include <unordered_map>

namespace xmd {
    model::model(const model &other) {
        *this = other;
    }

    model &model::operator=(const model &other) {
        std::unordered_map<residue const*, residue*> res_map;

        residues.clear();
        for (auto const& other_res: other.residues) {
            auto& res = residues.emplace_back(
                std::make_unique<residue>(*other_res));

            res_map[&*other_res] = &*res;
        }

        chains.clear();
        for (auto const& other_chain: other.chains) {
            auto& xmd_chain = chains.emplace_back(
                std::make_unique<chain>(*other_chain));

            for (residue*& res: xmd_chain->residues) {
                res = res_map[res];
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

        model_box = other.model_box;

        return *this;
    }

    model &model::operator+=(const model &m2) {
        auto m2_copy = m2;

        for (auto& res: m2_copy.residues)
            residues.emplace_back(std::move(res));
        for (auto& chain: m2_copy.chains)
            chains.emplace_back(std::move(chain));

        contacts.insert(contacts.end(),
            m2_copy.contacts.begin(), m2_copy.contacts.end());
        angles.insert(angles.end(),
            m2_copy.angles.begin(), m2_copy.angles.end());
        dihedrals.insert(dihedrals.end(),
            m2_copy.dihedrals.begin(), m2_copy.dihedrals.end());

        return *this;
    }

    model operator+(const model &m1, const model &m2) {
        model sum;
        sum += m1;
        sum += m2;
        return sum;
    }

    std::vector<model::tether> model::tethers(std::optional<true_real> tether_len) const {
        size_t num_tethers = 0;
        for (auto const& chain_: chains) {
            auto num_residues = chain_->residues.size();
            if (num_residues > 0)
                num_tethers += num_residues - 1;
        }

        std::vector<tether> tethers_(num_tethers);
        size_t tether_idx = 0;
        for (auto const& chain_: chains) {
            for (size_t res_idx = 0; res_idx+1 < chain_->residues.size(); ++res_idx) {
                auto& tether_ = tethers_[tether_idx];
                tether_.res1 = chain_->residues[res_idx];
                tether_.res2 = chain_->residues[res_idx+1];

                auto nat_length = (tether_.res2->pos - tether_.res1->pos).norm();
                tether_.length = tether_len.value_or(nat_length);

                ++tether_idx;
            }
        }

        return tethers_;
    }
}
