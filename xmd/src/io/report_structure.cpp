#include "io/report_structure.h"
#include <yaml-cpp/yaml.h>
#include <xmd/params/param_file.h>
#include <xmd/utils/text.h>
#include <xmd/files/csv.h>
#include <fstream>
#include <vector>
#include <xmd/utils/units.h>

namespace xmd {
    void report_structure::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        path_fmt = vm_inst.find_or_emplace<std::string>("rep_struct_path",
            params["report structure"]["path format"].as<std::string>());
        period = vm_inst.find_or_emplace<real>("rep_struct_period",
            params["report structure"]["exec period"].as<quantity>());

        nat_active_thr = vm_inst.find_or_emplace<real>("nat_active_thr",
            params["native contacts"]["active threshold factor"].as<real>());

        xmd_model = &vm_inst.find<xmd::model>("model");
        res_map = &vm_inst.find<res_map_t>("res_map");
        r = vm_inst.find<vec3r_vector>("r").to_array();

        if (vm_inst.has("sync"))
            sync = vm_inst.find<qa::sync_data_vector>("sync").to_array();

        if (vm_inst.has("qa_contacts"))
            qa_cont = &vm_inst.find<qa::contact_set>("qa_contacts");

        t = &vm_inst.find<real>("t");
        last_t = &vm_inst.find_or_emplace<real>("rep_struct_last_t",
            std::numeric_limits<real>::lowest());
    }

    static std::ostream& emit_csv(std::ostream& os, csv_file const& csv) {
        os << "|-";
        if (csv.header)
            os << "\n  " << csv.header->header_record;
        for (auto const& record: csv.records) {
            os << "\n  " << record;
        }
        return os;
    }

    void report_structure::operator()() const {
        if (*t - *last_t > period) {
            std::stringstream yaml_ss;

            csv_file res_csv;

            std::vector<std::string> res_csv_header = { "idx", "x", "y", "z" };
            if (sync) {
                std::vector<std::string> sync_fields = {
                    "back", "side-all", "side-polar", "side-hydrophobic" };
                res_csv_header.insert(res_csv_header.end(),
                    sync_fields.begin(), sync_fields.end());
            }
            res_csv.set_header(res_csv_header);

            for (int idx = 0; idx < (int)xmd_model->residues.size(); ++idx) {
                auto& record = res_csv.add_record();
                record["idx"] = std::to_string(idx);

                auto r_ = r[idx];
                record["x"] = std::to_string(r_.x());
                record["y"] = std::to_string(r_.y());
                record["z"] = std::to_string(r_.z());

                if (sync) {
                    auto sync_ = sync.value()[idx];
                    record["back"] = std::to_string(sync_.back);
                    record["side-all"] = std::to_string(sync_.side_all);
                    record["side-polar"] = std::to_string(
                        sync_.side_polar);
                    record["side-hydrophobic"] = std::to_string(
                        sync_.side_hydrophobic);
                }
            }

            yaml_ss << "residues: ";
            emit_csv(yaml_ss, res_csv);

            csv_file nat_cont_csv;
            nat_cont_csv.set_header({ "idx", "i1", "i2", "type", "nat_dist",
                                      "cur_dist", "active" });
            for (int idx = 0; idx < (int)xmd_model->contacts.size(); ++idx) {
                auto& record = nat_cont_csv.add_record();
                auto& cont = xmd_model->contacts[idx];

                record["idx"] = std::to_string(idx);
                auto i1 = res_map->at(cont.res1), i2 = res_map->at(cont.res2);
                record["i1"] = std::to_string(i1);
                record["i2"] = std::to_string(i2);

                std::string type;
                switch (cont.type) {
                case model::UNKNOWN: type = ""; break;
                case model::BACK_BACK: type = "BACK_BACK"; break;
                case model::BACK_SIDE: type = "BACK_SIDE"; break;
                case model::SIDE_BACK: type = "SIDE_BACK"; break;
                case model::SIDE_SIDE: type = "SIDE_SIDE"; break;
                case model::NAT_SS: type = "NAT_SS"; break;
                }
                record["type"] = type;

                record["nat_dist"] = std::to_string(cont.length);

                auto cur_dist = norm(r[i1] - r[i2]);
                record["cur_dist"] = std::to_string(cur_dist / angstrom);

                auto is_active = cur_dist < nat_active_thr * pow(2.0, -1.0/6.0) * cont.length;
                record["active"] = std::to_string(is_active);
            }

            yaml_ss << "\n\n" << "native contacts: ";
            emit_csv(yaml_ss, nat_cont_csv);

            if (qa_cont) {
                csv_file qa_cont_csv;
                qa_cont_csv.set_header({ "idx", "i1", "i2", "type",
                                         "t0", "status" });

                auto& qa_cont_ = *qa_cont.value();
                for (int idx = 0; idx < qa_cont_.extent(); ++idx) {
                    auto& record = qa_cont_csv.add_record();
                    record["idx"] = std::to_string(idx);
                    record["i1"] = std::to_string(qa_cont_.i1[idx]);
                    record["i2"] = std::to_string(qa_cont_.i2[idx]);

                    std::string type;
                    switch ((short)qa_cont_.type[idx]) {
                    case (short)qa::contact_type::BACK_BACK():
                        type = "BACK_BACK";
                        break;
                    case (short)qa::contact_type::BACK_SIDE():
                        type = "BACK_SIDE";
                        break;
                    case (short)qa::contact_type::SIDE_BACK():
                        type = "SIDE_BACK";
                        break;
                    default:
                        type = "SIDE_SIDE";
                        break;
                    }
                    record["type"] = type;

                    record["t0"] = std::to_string(qa_cont_.ref_time[idx]);

                    std::string status;
                    switch (qa_cont_.status[idx]) {
                    case qa::FORMING_OR_FORMED:
                        status = "FORMING_OR_FORMED";
                        break;
                    case qa::BREAKING:
                        status = "BREAKING";
                        break;
                    }
                    record["status"] = status;
                }

                yaml_ss << "\n\n" << "QA contacts: ";
                emit_csv(yaml_ss, qa_cont_csv);
            }

            csv_file angles_csv;
            angles_csv.set_header({ "idx", "i1", "i2", "i3", "nat_theta",
                                    "cur_theta" });
            for (int idx = 0; idx < (int)xmd_model->angles.size(); ++idx) {
                auto& record = angles_csv.add_record();
                auto& angle = xmd_model->angles[idx];

                auto i1 = res_map->at(angle.res1);
                auto i2 = res_map->at(angle.res2);
                auto i3 = res_map->at(angle.res3);

                record["idx"] = std::to_string(idx);
                record["i1"] = std::to_string(i1);
                record["i2"] = std::to_string(i2);
                record["i3"] = std::to_string(i3);
                record["nat_theta"] = angle.theta.has_value()
                    ? std::to_string(angle.theta.value()) : "";

                auto r1 = r[i1], r2 = r[i2], r3 = r[i3];
                auto r12 = r2 - r1, r23 = r3 - r2;
                auto r12_rn = norm_inv(r12), r23_rn = norm_inv(r23);
                auto cos_theta = -dot(r12, r23) * r12_rn * r23_rn;
                auto theta = acos(cos_theta);
                record["cur_theta"] = std::to_string(theta);
            }

            yaml_ss << "\n\n" << "angles: ";
            emit_csv(yaml_ss, angles_csv);

            csv_file dihedrals_csv;
            dihedrals_csv.set_header({ "idx", "i1", "i2", "i3", "i4",
                                       "nat_phi", "cur_phi" });
            for (int idx = 0; idx < (int)xmd_model->dihedrals.size(); ++idx) {
                auto& record = dihedrals_csv.add_record();
                auto& dihedral = xmd_model->dihedrals[idx];

                auto i1 = res_map->at(dihedral.res1);
                auto i2 = res_map->at(dihedral.res2);
                auto i3 = res_map->at(dihedral.res3);
                auto i4 = res_map->at(dihedral.res4);

                record["idx"] = std::to_string(idx);
                record["i1"] = std::to_string(i1);
                record["i2"] = std::to_string(i2);
                record["i3"] = std::to_string(i3);
                record["i4"] = std::to_string(i4);
                record["nat_phi"] = dihedral.phi.has_value()
                    ? std::to_string(dihedral.phi.value() / rad) : "";

                auto r1 = r[i1], r2 = r[i2], r3 = r[i3], r4 = r[i4];
                auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
                auto x12_23 = cross(r12, r23), x23_34 = cross(r23, r34);

                auto x12_23_rn = norm_inv(x12_23), x23_34_rn = norm_inv(x23_34);
                auto x12_23_u = x12_23 * x12_23_rn, x23_34_u = x23_34 * x23_34_rn;

                auto cos_phi = dot(x12_23_u, x23_34_u);
                auto phi = acos(cos_phi);
                if (dot(x12_23, r34) < 0.0f) phi = -phi;
                record["cur_phi"] = std::to_string(phi);
            }

            yaml_ss << "\n\n" << "dihedrals: ";
            emit_csv(yaml_ss, dihedrals_csv);

            std::filesystem::path file_path = format(path_fmt.c_str(), *t);
            create_directory(file_path.parent_path());
            std::ofstream structure_file(file_path, std::ios::trunc);
            structure_file << yaml_ss.str();

            *last_t = *t;
        }
    }
}