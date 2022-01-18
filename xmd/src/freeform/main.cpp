#include "freeform/main.h"
#include "freeform/params.h"
#include "defaults.h"
#include <xmd/utils/yaml.h>
#include <string>
#include <iostream>
#include <xmd/model/model.h>
#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <xmd/random/rand_gen.h>
#include <fstream>
#include <unordered_map>
#include <xmd/store/store.h>
#include <optional>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/reset_thread_vf.h>
#include <xmd/dynamics/reduce_vf.h>
#include <xmd/dynamics/lang_pc.h>
#include <xmd/forces/all.h>
#include <xmd/io/show_progress_bar.h>
#include <xmd/io/export_pdb.h>
#include <xmd/nl/verify.h>
#include <xmd/io/report_stats.h>
#include <xmd/io/report_structure.h>
#include <xmd/nl/legacy_update.h>
#include <experimental/type_traits>

namespace xmd {
    template<typename T>
    class kernel {
    public:
        T& value() {
            if (!maybe_v.has_value())
                maybe_v = T();
            return maybe_v.value();
        }

        T const& value() const {
            return maybe_v.value();
        }

        operator bool() const {
            return maybe_v.has_value();
        }

        void operator()() const {
            template <typename T>
            using detector = decltype(std::declval<T const&>()());

            using namespace std::experimental;
            if constexpr (is_detected_v<detector, T>)
                value()();
        }

        void omp_async() const {
            template <typename T>
            using detector = decltype(std::declval<T const&>().omp_async());

            using namespace std::experimental;
            if constexpr (is_detected_v<detector, T>)
                value().omp_async();
        }

        void omp_prep() const {
            template <typename T>
            using detector = decltype(std::declval<T const&>().omp_prep());

            using namespace std::experimental;
            if constexpr (is_detected_v<detector, T>)
                value().omp_prep();
        }

    private:
        std::optional<T> maybe_v;
    };

    int freeform_main::operator()(int argc, char **argv) {
        std::vector<std::string> argv_s(argc);
        for (int idx = 0; idx < argc; ++idx)
            argv_s[idx] = argv[idx];

        auto print_help = [&]() -> void {
            std::cout << "Usage: " << argv_s[0] << " [--help|file]" << '\n';
            std::cout << '\t' << "file" << '\t' << "YAML parameter file" << '\n';
            std::cout << '\t' << "--help" << '\t' << "display this help message";
        };

        if (argc == 2 && argv_s[1] == "--help") {
            print_help();
            return EXIT_SUCCESS;
        }
        else if (argc > 2) {
            print_help();
            return EXIT_FAILURE;
        }

        yaml_fs_node params_fs_node;
        if (argc == 2) {
            auto param_path = argv_s[1];
            auto overrides = YAML::LoadFile(param_path);
            auto params_node = merge_yaml(defaults_yml(), overrides);
            params_fs_node = yaml_fs_node(params_node, param_path);
        }
        else {
            params_fs_node = yaml_fs_node(defaults_yml(), ".");
        }

        freeform_params params(params_fs_node);

        store st;

        auto& gen = st.make_temp<xmd::rand_gen>("gen", params.general.seed);

        xmd::model model;
        auto& model_file_v = params.model.file;
        if (std::holds_alternative<model_params::seqfile_params>(model_file_v)) {
            auto& seqfile_p = std::get<model_params::seqfile_params>(model_file_v);
            auto seqfile  = seq_file(seqfile_p.path);
            model = seqfile.to_model();
        }
        else if (std::holds_alternative<model_params::pdbfile_params>(model_file_v)) {
            auto& pdbfile_p = std::get<model_params::pdbfile_params>(model_file_v);
            auto stream = std::ifstream(pdbfile_p.path);
            auto pdbfile = pdb(std::move(stream));
            model = pdbfile.to_model();
        }

        if (params.model.morph_into_saw.has_value()) {
            auto& saw_p = params.model.morph_into_saw.value();
            model.morph_into_saw(gen, saw_p.bond_distance, saw_p.residue_density,
                saw_p.infer_box);
        }

        using res_map_t  = std::unordered_map<xmd::model::residue*, int>;
        res_map_t res_map;

        int res_idx = 0;
        for (auto const& res: model.residues)
            res_map[res.get()] = res_idx++;

        auto& num_res = st.make_temp<int>("num_res", model.residues.size());

        auto& r = st.make_saved<vector<vec3r>>("r", [&]() -> auto {
            vector<vec3r> r_(num_res);
            for (auto const& res: model.residues)
                r_[res_map.at(res.get())] = res->pos;
            return r_;
        });

        auto& atype = st.make_saved<vector<amino_acid>>("atype", [&]() -> auto {
            vector<amino_acid> atype_(num_res);
            for (auto const& res: model.residues)
                atype_[res_map.at(res.get())] = res->type;
        });

        auto& mass = st.make_temp<vector<real>>("mass", num_res, (real)1.0);
        auto& box = st.make_saved<xmd::box>("box", [&]() -> auto {
            return model.model_box;
        });

        auto& prev = st.make_saved<vector<int>>("prev", [&]() -> auto {
            vector<int> prev_(num_res, -1);
            for (auto const& res: model.residues) {
                if (res->seq_idx > 0) {
                    auto prev_ptr = res->parent->residues[res->seq_idx-1];
                    prev_[res_map.at(res.get())] = res_map.at(prev_ptr);
                }
            }
            return prev_;
        });

        auto& next = st.make_saved<vector<int>>("next", [&]() -> auto {
            vector<int> next_(num_res, -1);
            for (auto const& res: model.residues) {
                auto num_res_in_chain = (int)res->parent->residues.size();
                if (res->seq_idx < num_res_in_chain-1) {
                    auto next_ptr = res->parent->residues[res->seq_idx+1];
                    next_[res_map.at(res.get())] = res_map.at(next_ptr);
                }
            }
            return next_;
        });

        auto& chain_idx = st.make_saved<vector<int>>("chain_idx", [&]() -> auto {
            vector<int> chain_idx_(num_res, -1);
            for (auto const& res: model.residues) {
                chain_idx_[res_map.at(res.get())] = res->parent->chain_idx;
            }
            return chain_idx_;
        });

        auto& seq_idx = st.make_saved<vector<int>>("seq_idx", [&]() -> auto {
            vector<int> seq_idx_(num_res, -1);
            for (auto const& res: model.residues) {
                seq_idx_[res_map.at(res.get())] = res->seq_idx;
            }
            return seq_idx_;
        });

        auto& t = st.make_saved<real>("t", []() -> auto {
            return (real) 0.0;
        });

        auto& F = st.make_saved<vector<vec3r>>("F", [&]() -> auto {
            return vector<vec3r>(num_res, (true_real)0.0);
        });

        kernel<lang_pc_step> lang_pc;
        if (auto& langevin_p = params.langevin; langevin_p.enabled) {
            auto& ref = lang_pc.value();
            ref.temperature = langevin_p.temperature;
            ref.t = &t;
            ref.gamma_factor = langevin_p.gamma_factor;
            ref.gen = &gen;
            ref.mass = mass.data();

            auto mass_inv = st.make_temp<vector<real>>("mass_inv", num_res);
            for (int idx = 0; idx < num_res; ++idx)
                mass_inv[idx] = (real)1.0 / mass[idx];
            ref.mass_inv = mass_inv.data();

            auto mass_rsqrt = st.make_temp<vector<real>>("mass_rsqrt", num_res);
            for (int idx = 0; idx < num_res; ++idx)
                mass_rsqrt[idx] = (real)1.0 / sqrt(mass[idx]);
            ref.mass_rsqrt = mass_rsqrt.data();

            ref.v = st.make_saved<vector<vec3r>>("v", [&]() -> auto {
                return vector<vec3r>(num_res, (real)0.0);
            }).data();

            auto y_init = [&]() -> auto {
                return vector<vec3tr>(num_res, (true_real)0.0);
            };

            ref.y0 = st.make_saved<vector<vec3tr>>("y0", [&]() -> auto {
                auto y0_ = vector<vec3tr>(num_res);
                for (int idx = 0; idx < num_res; ++idx)
                    y0_[idx] = r[idx];
                return y0_;
            }).data();

            ref.y1 = st.make_saved<vector<vec3tr>>("y1", y_init);
            ref.y2 = st.make_saved<vector<vec3tr>>("y2", y_init);
            ref.y3 = st.make_saved<vector<vec3tr>>("y3", y_init);
            ref.y4 = st.make_saved<vector<vec3tr>>("y4", y_init);
            ref.y5 = st.make_saved<vector<vec3tr>>("y5", y_init);

            ref.true_t = &st.make_saved<true_real>("true_t", []() -> auto {
                return (true_real)0.0;
            });

            ref.F = F.data();
        }

        auto& V = st.make_temp<real>("V", (real)0.0);

        reset_vf reset_vf_;
        reset_vf_.F = F.data();
        reset_vf_.V = &V;

        reduce_vf reduce_vf_;
        reduce_vf_.F = F.data();
        reduce_vf_.V = &V;

        kernel<export_pdb> export_pdb_;
        if (auto& export_pdb_p = params.export_pdb; export_pdb_p.enabled) {
            auto& ref = export_pdb_.value();
            ref.out_file_path = export_pdb_p.path;
            ref.period = export_pdb_p.exec_period;
            ref.r = r.data();
            ref.ref_model = &model;
            ref.res_map = &res_map;
            ref.serial = &st.make_saved<int>("pdb_model_serial",
                []() -> auto { return 1; });
            ref.last_t = &st.make_saved<real>("export_pdb.last_t",
                []() -> auto { return std::numeric_limits<real>::lowest(); });
            ref.t = &t;
        }

        kernel<show_progress_bar> show_pbar_;
        if (auto& show_pbar_p = params.progress_bar; show_pbar_p.enabled) {
            auto& ref = show_pbar_.value();
            ref.width = show_pbar_p.width;
            ref.total_time = params.general.total_time;
            ref.period = show_pbar_p.update_period;
            ref.t = &t;
            ref.V = &V;
            ref.first_time = &st.make_saved<bool>("show_pbar.first_time",
                []() -> auto { return true; });
            ref.start_wall_time = &st.make_temp<show_progress_bar::time_point_t>(
                "show_pbar.start_wall_time");
            ref.start_t = &st.make_saved<real>("show_pbar.start_t",
                []() -> auto { return 0.0; });
            ref.last_t = &st.make_saved<real>("show_pbar.last_t",
                []() -> auto { return std::numeric_limits<real>::lowest(); });
        }

        kernel<report_stats> report_stats_;
        if (auto& report_stats_p = params.report_stats; report_stats_p.enabled) {
            auto& ref = report_stats_.value();
            ref.period = report_stats_p.exec_period;
            ref.csv_path = report_stats_p.path;
            ref.t = &t;
            ref.last_t =  &st.make_saved<real>("report_stats.last_t",
                []() -> auto { return std::numeric_limits<real>::lowest(); });
            ref.first_time = &st.make_saved<bool>("report_stats.first_time",
                []() -> auto { return true; });
            ref.V = &V;

            ref.comp_tot_ene_t.v = lang_pc.value().v;;
            ref.comp_tot_ene_t.mass = mass.data();
            ref.comp_tot_ene_t.num_particles = num_res;
            ref.comp_tot_ene_t.V = &V;
            ref.comp_tot_ene_t.K = &st.make_temp<real>("total_energy.K", (real)0.0);
            ref.comp_tot_ene_t.E = &st.make_temp<real>("total_energy.E", (real)0.0);

            ref.comp_asph_t.asphericity = &st.make_temp<real>(
                "comp_asph.asphericity", (real)0.0);
            ref.comp_asph_t.r = r.data();
            ref.comp_asph_t.mass = mass.data();
            ref.comp_asph_t.num_particles = num_res;

            ref.comp_gyr_t.gyration_radius = &st.make_temp<real>(
                "comp_gyr.gyration_radius", (real)0.0);
            ref.comp_gyr_t.num_particles = num_res;
            ref.comp_gyr_t.mass = mass.data();
            ref.comp_gyr_t.r = r.data();
        }

        kernel<report_structure> rep_struct_;
        if (auto& rep_struct_p = params.report_structure; rep_struct_p.enabled) {
            auto& ref = rep_struct_.value();

            ref.path_fmt = rep_struct_p.path_fmt;
            ref.period = rep_struct_p.exec_period;
            ref.nat_active_thr = params.nat_cont.active_threshold;
            ref.xmd_model = &model;
            ref.res_map = &res_map;
            ref.r = r.data();
            ref.t = &t;
            ref.last_t = &st.make_saved<real>("rep_struct.last_t",
                []() -> auto { return std::numeric_limits<real>::lowest(); });
        }

        auto& nl_data = st.make_temp<nl::nl_data>("nl.data");
        auto& nl_invalid = st.make_temp<bool>("nl.invalid", true);
        auto& max_cutoff = st.make_saved<real>("max_cutoff",
            []() -> auto { return 0.0; });

        nl::legacy_update legacy_update_;
        legacy_update_.pad_factor = params.nl.pad_factor;
        legacy_update_.r = r.data();
        legacy_update_.box = &box;
        legacy_update_.t = &t;
        legacy_update_.chain_idx = chain_idx.data();
        legacy_update_.seq_idx = seq_idx.data();
        legacy_update_.num_particles = num_res;
        legacy_update_.data = &nl_data;
        legacy_update_.data->orig_r = vector<vec3r>(legacy_update_.num_particles);
        legacy_update_.invalid = &nl_invalid;
        legacy_update_.max_cutoff = &max_cutoff;

        nl::verify verify_;
        verify_.r = r.data();
        verify_.data = &nl_data;
        verify_.box = &box;
        verify_.invalid = &nl_invalid;
        verify_.first_time = &st.make_saved<bool>("verify.first_time",
            []() -> auto { return true; });
        verify_.num_particles = num_res;

        kernel<eval_chiral_forces> eval_chir_;
        if (auto& chir_p = params.chir; chir_p.enabled) {
            auto& ref = eval_chir_.value();
            ref.e_chi = chir_p.e_chi;

            auto quads_init = [&]() -> auto {
                vector<chiral_quad> quads_;
                for (auto const& dihedral: xmd_model.dihedrals) {
                    auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                        i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];

                    vec3r nat_r1 = dihedral.res1->pos, nat_r2 = dihedral.res2->pos,
                        nat_r3 = dihedral.res3->pos, nat_r4 = dihedral.res4->pos;

                    auto nat_r12 = nat_r2 - nat_r1, nat_r23 = nat_r3 - nat_r2,
                        nat_r34 = nat_r4 - nat_r3;

                    auto nat_factor = ipow<3>(norm_inv(nat_r23));
                    auto nat_chir = dot(nat_r12, cross(nat_r23, nat_r34)) * nat_factor;

                    quads_.emplace_back(i1, i2, i3, i4, nat_factor, nat_chir);
                }
                return quads_;
            };
            ref.quads = &st.make_saved<vector<chiral_quad>>(
                "eval_chir.quads", quads_init);
        }

        kernel<eval_tether_forces> tethers_;
        if (auto& tethers_p = params.tether; tethers_p.enabled) {
            auto& ref = tethers_.value();
            ref.H1 = tethers_p.H1;
            ref.H2 = tethers_p.H2;
            ref.def_length = tethers_p.def_length;
            ref.r = r.data();

            auto tethers_init = [&]() -> auto {
                vector<tether_pair> tethers_;
                for (auto const& tether: xmd_model.tethers) {
                    auto i1 = res_map[tether.res1], i2 = res_map[tether.res2];
                    auto nat_dist = (real)tether.length.value_or(def_length);
                    tethers_.emplace_back(i1, i2, nat_dist);
                }
                return tethers_;
            };
            ref.tethers = st.make_saved<vector<tether_pair>>(
                "tethers", tethers_init);
        }

        kernel<eval_native_angle_forces> nat_ang_;
        if (auto& nat_ang_p = params.nat_ang; nat_ang_p.enabled) {
            auto& ref = nat_ang_.value();
            ref.k = nat_ang_p.k;
            ref.r = r.data();

            auto angles_init = [&]() -> auto {
                vector<nat_ang> angles_;

                for (auto const& angle: model.angles) {
                    if (angle.theta.has_value()) {
                        auto i1 = res_map[angle.res1], i2 = res_map[angle.res2],
                            i3 = res_map[angle.res3];
                        auto nat_theta = (real)angle.theta.value();

                        angles_.emplace_back(i1, i2, i3, nat_theta);
                    }
                }

                return angles_;
            };
            ref.angles = st.make_saved<vector<nat_ang>>(
                "nat_ang.angles", angles_init).view();
        }

        auto native_dih_init = [&]() -> auto {
            vector<nat_dih> dihedrals_;

            for (auto const& dihedral: xmd_model.dihedrals) {
                if (dihedral.phi.has_value()) {
                    auto i1 = res_map[dihedral.res1], i2 = res_map[dihedral.res2],
                        i3 = res_map[dihedral.res3], i4 = res_map[dihedral.res4];
                    auto nat_phi = (real)dihedral.phi.value();

                    dihedrals_.emplace_back(i1, i2, i3, i4, nat_phi);
                }
            }

            return dihedrals_;
        };

        kernel<eval_cnd_forces> cnd_;
        if (auto& cnd_p = params.cnd; cnd_p.enabled) {
            auto& ref = cnd_.value();
            ref.CDA = cnd_p.CDA;
            ref.CDB = cnd_p.CDB;
            ref.r = r.data();
            ref.dihedrals = st.make_saved<vector<nat_dih>>(
                "nat_dih.dihedrals", native_dih_init).view();
        }

        kernel<eval_snd_forces> snd_;
        if (auto& snd_p = params.snd; snd_p.enabled) {
            auto& ref = snd_.value();
            ref.CDH = snd_p.CDH;
            ref.r = r.data();
            ref.dihedrals = st.make_saved<vector<nat_dih>>(
                "nat_dih.dihedrals", native_dih_init).view();
        }

        kernel<eval_pauli_exclusion_forces> eval_pauli_;
        kernel<update_pauli_pairs> update_pauli_;
        if (auto& pauli_p = params.pauli; pauli_p.enabled) {
            auto& pauli_pairs = st.make_saved<vector<pauli_pair>>(
                "pauli.pairs", []() -> auto { return vector<pauli_pair>(); });

            auto& eval_ref = eval_pauli_.value();
            eval_ref.r_excl = pauli_p.r_excl;
            eval_ref.depth = pauli_p.depth;
            eval_ref.r = r.data();
            eval_ref.box = &box;
            eval_ref.pairs = &pauli_pairs;

            auto& update_ref = update_pauli_.value();
            update_ref.r = r.data();
            update_ref.box = &box;
            update_ref.nl = &nl_data;
            update_ref.pairs = &pauli_pairs;

            max_cutoff = max(max_cutoff, pauli_p.r_excl);
        }

        kernel<eval_go_forces> eval_go_;
        kernel<update_go_contacts> update_go_;
        if (auto& nat_cont_p = params.nat_cont; nat_cont_p.enabled) {
            auto& cur_nat_cont = st.make_saved<vector<nat_cont>>(
                "nat_cont.cur", []() -> auto { return vector<nat_cont>(); });

            auto all_nat_cont_init = [&]() -> auto {
                vector<nat_cont> all_nat_cont_;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type != model::NAT_SS) {
                        auto i1 = res_map[cont.res1], i2 = res_map[cont.res2];
                        auto nat_dist = (real)cont.length;
                        all_nat_cont_.emplace_back(i1, i2, nat_dist);
                    }
                }
                return all_nat_cont_;
            };
            auto& all_nat_cont = st.make_saved<vector<nat_cont>>(
                "nat_cont.all", all_nat_cont_init);

            auto all_nl_nat_cont_init = [&]() -> auto {
                vector<nl::nl_pair> all_nl_nat_cont_;
                for (auto const& nat_cont: all_nat_cont)
                    all_nl_nat_cont_.emplace_back(nat_cont.i1(), nat_cont.i2());
                return all_nl_nat_cont_;
            };
            legacy_update_.all_nat_cont = st.make_saved<vector<nl::nl_pair>>(
                "nat_cont.all_nl", all_nl_nat_cont_init).view();

            auto& eval_ref = eval_go_.value();
            eval_ref.depth = nat_cont_p.lj_depth;
            eval_ref.box = &box;
            eval_ref.contacts = &cur_nat_cont;
            eval_ref.r = r.data();

            auto& update_ref = update_go_.value();
            update_ref.r = r.data();
            update_ref.box = &box;
            update_ref.nl = &nl_data;
            update_ref.all_contacts = &all_nat_cont;
            update_ref.contacts = &cur_nat_cont;

            real cutoff = 0.0;
            for (int cont_idx = 0; cont_idx < all_contacts->size(); ++cont_idx) {
                auto cont_dist = all_contacts->at(cont_idx).nat_dist();
                cutoff = max(cutoff, lj::cutoff(cont_dist));
            }

            max_cutoff = max(max_cutoff, cutoff);
        }

        kernel<eval_nat_ssbond_forces> eval_ss_;
        kernel<update_nat_ssbonds> update_ss_;
        if (auto& nat_ss_p = params.nat_ss; nat_ss_p.enabled) {
            auto& cur_nat_ss = st.make_saved<vector<nat_ss>>(
                "nat_ss.cur", []() -> auto { return vector<nat_Ss>(); });

            auto all_nat_ss_init = [&]() -> auto {
                vector<nat_ss> all_nat_ss_;
                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type == model::NAT_SS) {
                        auto i1 = res_map[cont.res1], i2 = res_map[cont.res2];
                        all_nat_ss_.emplace_back(i1, i2);
                    }
                }
                return all_nat_ss_;
            };
            auto& all_nat_ss = st.make_saved<vector<nat_ss>>(
                "nat_ss.all", all_nat_ss_init);

            auto all_nl_nat_ss_init = [&]() -> auto {
                vector<nl::nl_pair> all_nl_nat_ss_;
                for (auto const& nat_ss: all_nat_ss)
                    all_nl_nat_ss_.emplace_back(nat_ss.i1(), nat_ss.i2());
                return all_nl_nat_ss_;
            };
            legacy_update_.all_nat_ss = st.make_saved<vector<nl::nl_pair>>(
                "nat_ss.all_nl", all_nl_nat_ss_init).view();

            auto& eval_ref = eval_ss_.value();
            eval_ref.H1 = nat_ss_p.H1;
            eval_ref.nat_r = nat_ss_p.equil_dist;
            eval_ref.box = &box;
            eval_ref.ssbonds = &cur_nat_ss;
            eval_ref.r = r.data();

            auto& update_ref = update_ss_.value();
            update_ref.r = r.data();
            update_ref.box = &box;
            update_ref.nl = &nl_data;
            update_ref.all_ssbonds = &all_nat_ss;
            update_ref.ssbonds = &cur_nat_ss;

            auto cutoff = harmonic::cutoff(eval_ref.nat_r);
            max_cutoff = max(max_cutoff, cutoff);
        }

        kernel<eval_const_es_forces> eval_const_es_;
        kernel<update_const_es> update_const_es_;

        kernel<eval_relative_es_forces> eval_rel_es_;
        kernel<update_relative_es> update_rel_es_;

        if (params.const_es.enabled || params.rel_es.enabled) {
            update_es_base& base_ref = params.const_es.enabled ?
                update_const_es_.value() : update_rel_es_.value();

            for (auto const& aa: amino_acid::all())
                base_ref.q[(short)aa] = params.aa_data[aa].charge;

            base_ref.r = r.data();
            base_ref.box = &box;
            base_ref.nl = &nl_data;
            base_ref.pairs = &st.make_saved<vector<es_pair>>(
                "es.pairs", []() -> auto { return vector<es_pair>(); });

            if (auto& const_es_p = params.const_es; const_es_p.enabled) {
                auto& eval_ref = eval_const_es_.value();
                eval_ref.permittivity = const_es_p.permittivity;
                eval_ref.V_factor = 1.0/(4.0*M_PI*eval_ref.permittivity);
                eval_ref.screen_dist_inv = 1.0/const_es_p.screening_distance;
                eval_ref.r = r.data();
                eval_ref.box = &box;

                auto& update_ref = update_const_es_.value();
                update_ref.cutoff = 2.0 * const_es_p.screening_distance;

                max_cutoff = max(max_cutoff, update_ref.cutoff);
            }

            if (auto& rel_es_p = params.rel_es; rel_es_p.enabled) {
                auto& eval_ref = eval_rel_es_.value();
                eval_ref.factor = rel_es_p.factor;
                eval_ref.V_factor = 1.0/(4.0*M_PI*eval_ref.factor);
                eval_ref.screen_dist_inv = 1.0/rel_es_p.screening_distance;
                eval_ref.r = r.data();
                eval_ref.box = &box;

                auto& update_ref = update_rel_es_.value();
                update_ref.cutoff = 2.0 * rel_es_p.screening_distance;

                max_cutoff = max(max_cutoff, update_ref.cutoff);
            }
        }

        kernel<qa::precompute_nh> precomp_nh_;
        kernel<qa::sift_candidates> qa_sift_cand_;
        kernel<qa::process_candidates> qa_proc_cand_;
        kernel<qa::process_contacts> qa_proc_cont_;
        kernel<qa::update_free_pairs> qa_update_free_;

        if (auto& qa_p = params.qa; qa_p.enabled) {
            auto& free_pairs_ = st.make_saved<set<qa::free_pair>>(
                "qa.free_pairs", []() -> auto { return set<qa::free_pair>(); });
            auto& candidates_ = st.make_saved<set<qa::candidate>>(
                "qa.candidates", []() -> auto { return set<qa::candidate>(); });
            auto& contacts_ = st.make_saved<set<qa::contact>>(
                "qa.contacts", []() -> auto { return set<qa::contact>(); });

            auto sync_init = [&]() -> auto {
                vector<sync_data> sync_(num_res);
                for (int idx = 0; idx < num_res; ++idx) {
                    auto const& lim = aa_data_[atype[idx]].limits;
                    sync_.emplace_back((int8_t)lim.back, (int8_t)lim.side_all,
                        (int8_t)lim.side_polar, (int8_t)lim.side_hydrophobic);
                }
                return sync_;
            };
            auto& sync_ = st.make_saved<vector<qa::sync_data>>(
                "qa.sync", sync_init);

            auto& precomp_ref = precomp_nh_.value();
            precomp_ref.r = r.data();
            precomp_ref.num_particles = num_res;
            precomp_ref.n = st.make_saved<vector<vec3r>>(
                "qa.n", [&]() -> auto { return vector<vec3r>(num_res); }).data();
            precomp_ref.h = st.make_saved<vector<vec3r>>(
                "qa.h", [&]() -> auto { return vector<vec3r>(num_res); }).data();
            precomp_ref.prev = prev.data();
            precomp_ref.next = next.data();

            auto& sift_ref = qa_sift_cand_.value();
            sift_ref.min_abs_cos_hr = qa_p.min_cos_hr;
            sift_ref.min_abs_cos_hh = qa_p.min_cos_hh;
            sift_ref.max_cos_nr = qa_p.max_cos_nr;

            sift_ref.req_min_dist[(short)qa::contact_type::BACK_BACK()] =
                params.lj_vars.bb.r_min();
            sift_ref.req_min_dist[(short)qa::contact_type::BACK_SIDE()] =
                params.lj_vars.bs.r_min();
            sift_ref.req_min_dist[(short)qa::contact_type::SIDE_BACK()] =
                params.lj_vars.sb.r_min();

            for (auto const& aa1: amino_acid::all()) {
                for (auto const& aa2: amino_acid::all()) {
                    int ss_idx = (int)aa1 * amino_acid::NUM_AA + (int)aa2;
                    auto type_idx = (short)qa::contact_type::SIDE_SIDE(aa1, aa2);
                    sift_ref.req_min_dist[type_idx] = params.lj_vars.ss[ss_idx].r_max();
                }
            }

            for (auto const& aa: amino_acid::all())
                sift_ref.ptype[(short)aa] = params.aa_data[aa].polarization;

            sift_ref.candidates = &candidates_;
            sift_ref.atype = &atype;
            sift_ref.box = &box;
            sift_ref.r = r.data();
            sift_ref.free_pairs = &free_pairs_;
            sift_ref.sync = &sync_;
            sift_ref.n = precomp_ref.n;
            sift_ref.h = precomp_ref.h;

            auto& proc_cand_ref = qa_proc_cand_.value();
            proc_cand_ref.candidates = &candidates_;
            proc_cand_ref.contacts = &contacts_;
            proc_cand_ref.sync = &sync_;
            proc_cand_ref.free_pairs = &free_pairs_;
            proc_cand_ref.t = &t;

            auto& proc_cont_ref = qa_proc_cont_.value();
            proc_cont_ref.cycle_time = qa_p.phase_duration;
            proc_cont_ref.cycle_time_inv = 1.0 / proc_cont_ref.cycle_time;
            proc_cont_ref.breaking_factor = qa_p.breaking_factor;
            proc_cont_ref.t = &t;
            proc_cont_ref.factor = proc_cont_ref.breaking_factor *
                (real)pow(2.0f, -1.0f/6.0f);
            proc_cont_ref.sync = sync_.data();
            proc_cont_ref.contacts = &contacts_;
            proc_cont_ref.box = &box;
            proc_cont_ref.r = r.data();
            proc_cont_ref.free_pairs = &free_pairs_;

            auto& update_ref = qa_update_free_.value();
            update_ref.r = r.data();
            update_ref.box = &box;
            update_ref.nl = &nl_data;
            update_ref.pairs = &free_pairs_;

            real max_req_min_dist = 0.0;
            for (int idx = 0; idx < qa::contact_type::NUM_TYPES; ++idx)
                max_req_min_dist = max(max_req_min_dist, sift_ref.req_min_dist[idx]);

            max_cutoff = max(max_cutoff, max_req_min_dist);
        }

        kernel<pid::eval_pid_forces> eval_pid_;
        kernel<pid::update_pid_bundles> update_pid_;
        if (auto& pid_p = params.pid; pid_p.enabled) {
            auto& bundles = st.make_saved<vector<pid::pid_bundle>>(
                "pid.bundles", []() -> auto { return vector<pid::pid_bundle>(); });

            auto &eval_ref = eval_pid_.value();

            pid::lambda_version lambda;
            switch (pid_p.lambda_version) {
            case pid_params::lambda_version_t::COSINE:
                lambda = pid::lambda_version::COSINE;
                break;
            case pid_params::lambda_version_t::ALGEBRAIC:
                lambda = pid::lambda_version::ALGEBRAIC;
                break;
            case pid_params::lambda_version_t ::UNKNOWN:
                lambda = pid::lambda_version ::COSINE;
                break;
            }

            eval_ref.bb_plus_lam.version() = lambda;
            eval_ref.bb_plus_lam.alpha() = pid_p.bb_plus.alpha;
            eval_ref.bb_plus_lam.psi_0() = pid_p.bb_plus.psi_0;
            eval_ref.bb_plus_lj.r_min() = pid_p.bb_plus.r_min;
            eval_ref.bb_plus_lj.depth() = pid_p.bb_plus.depth;

            eval_ref.bb_minus_lam.version() = lambda;
            eval_ref.bb_minus_lam.alpha() = pid_p.bb_minus.alpha;
            eval_ref.bb_minus_lam.psi_0() = pid_p.bb_minus.psi_0;
            eval_ref.bb_minus_lj.r_min() = pid_p.bb_minus.r_min;
            eval_ref.bb_minus_lj.depth() = pid_p.bb_minus.depth;

            eval_ref.ss_lam.alpha() = pid_p.ss.alpha;
            eval_ref.ss_lam.psi_0() = pid_p.ss.psi_0;
            eval_ref.ss_ljs = params.lj_vars.ss.data();

            eval_ref.r = r.data();
            eval_ref.box = &box;
            eval_ref.bundles = &bundles;
            eval_ref.prev = prev.data();
            eval_ref.next = next.data();

            auto& update_ref = update_pid_.value();
            update_ref.r = r.data();
            update_ref.prev = prev.data();
            update_ref.next = next.data();
            update_ref.atype = atype.data();
            update_ref.box = &box;
            update_ref.nl = &nl_data;
            update_ref.bundles = &bundles;

            real cutoff = 0.0;
            cutoff = max(cutoff, params.lj_vars.bb.cutoff());
            cutoff = max(cutoff, params.lj_vars.bs.cutoff());
            cutoff = max(cutoff, params.lj_vars.sb.cutoff());
            for (int ss_idx = 0; ss_idx < params.lj_vars.ss.size(); ++ss_idx) {
                auto& ss = params.lj_vars.ss[ss_idx];
                cutoff = max(cutoff, ss.cutoff());
            }

            update_ref.cutoff = cutoff;
            max_cutoff = max(max_cutoff, cutoff);
        }

        kernel<eval_solid_wall_forces> eval_solid_;
        if (auto& solid_wall_p = params.solid_walls; solid_wall_p.enabled) {
            auto& ref = eval_solid_.value();
            ref.eps = solid_wall_p.depth;
            ref.cutoff = solid_wall_p.cutoff;
            ref.r = r.data();
            ref.num_particles = num_res;
            ref.box = &box;
            ref.wall_F = st.make_temp<vector<vec3r>>(
                "solid_wall.wall_F", solid_wall_p.planes.size());

            auto walls_init = [&]() -> auto {
                auto walls_ = vector<plane>();
                for (auto const& plane: solid_wall_p.planes)
                    walls_.emplace_back(plane);
                return walls_;
            };
            ref.walls = st.make_saved<vector<plane>>(
                "solid_wall.walls", walls_init);
        }

        kernel<eval_lj_attr_wall_forces> eval_lj_attr_;
        if (auto& lj_attr_p = params.lj_attr_walls; lj_attr_p.enabled) {
            auto& ref = eval_lj_attr_.value();
            ref.wall_min_dist = lj_attr_p.wall_min_dist;
            ref.breaking_factor = lj_attr_p.breaking_factor;
            ref.factor = ref.breaking_factor * pow(2.0, -1.0/6.0);
            ref.cycle_time = lj_attr_p.phase_duration;
            ref.cycle_time_inv = 1.0 / ref.cycle_time;
            ref.r = r.data();
            ref.box = &box;
            ref.num_particles = num_res;
            ref.t = &t;
            ref.wall_F = st.make_temp<vector<vec3r>>(
                "lj_attr.wall_F", solid_wall_p.planes.size());

            auto pairs_init = [&]() -> auto {
                vector<lj_attr_pair> pairs_;
                for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                    for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
                        pairs_.emplace_back(part_idx, wall_idx, FREE,
                            vec3r::Zero(),0.0f);
                    }
                }
                return pairs_;
            };
            ref.pairs = st.make_saved<vector<lj_attr_pair>>(
                "lj_attr.pairs", pairs_init).view();

            auto walls_init = [&]() -> auto {
                auto walls_ = vector<plane>();
                for (auto const& plane: lj_attr_p.planes)
                    walls_.emplace_back(plane);
                return walls_;
            };
            ref.walls = st.make_saved<vector<plane>>(
                "lj_attr.walls", walls_init).view();
        }

        kernel<eval_velocity_afm_forces> eval_vel_afm_;
        if (auto& vel_afm_p = params.vel_afm; vel_afm_p.enabled) {
            auto& ref = eval_vel_afm_.value();
            ref.afm_force.H1 = vel_afm_p.H1;
            ref.afm_force.H2 = vel_afm_p.H2;
            ref.r = r.data();
            ref.t = &t;

            auto afm_tips_init = [&]() -> auto {
                vector<vel_afm_tip> afm_tips;
                for (auto const& tip: vel_afm_p.afm_tips) {
                    vec3r orig = tip.origin.has_value() ?
                        tip.origin.value() : r[pulled_idx];
                    afm_tips.emplace_back(tip.pulled_idx, orig, tip.v);
                }
                return afm_tips;
            };
            ref.afm_tips = st.make_saved<vector<vel_afm_tip>>(
                "vel_afm.tips", afm_tips_init).view();
        }

        kernel<eval_force_afm_forces> eval_force_afm_;
        if (auto& force_afm_p = params.force_afm; force_afm_p.enabled) {
            auto& ref = eval_force_afm_.value();

            auto afm_tips_init = [&]() -> auto {
                vector<force_afm_tip> afm_tips;
                for (auto const& tip: force_afm_p.afm_tips) {
                    afm_tips.emplace_back(tip.pulled_idx, tip.F);
                }
                return afm_tips;
            };
            ref.afm_tips = st.make_saved<vector<force_afm_tip>>(
                "force_afm.tips", afm_tips_init).view();
        }

#pragma omp parallel default(none) firstprivate(lang_pc,reset_vf_,reduce_vf_,\
export_pdb_,show_pbar_,report_stats_,rep_struct_,legacy_update_,verify_,\
eval_chir_,tethers_,nat_ang_,cnd_,snd_,eval_pauli_,update_pauli_,eval_go_,\
update_go_,eval_ss_,update_ss_,eval_const_es_,update_const_es_,eval_rel_es_,\
update_rel_es_,precomp_nh_,qa_sift_cand_,qa_proc_cand_,qa_proc_cont_,\
qa_update_free_,eval_pid_,update_pid_,eval_solid_,eval_lj_attr_,\
eval_vel_afm_,eval_force_afm_) shared(st,num_res,nl_invalid,params)
        {
            real thread_V = 0.0;
            vector<vec3r> thread_F(num_res, vec3r::Zero());

            reset_thread_vf reset_thread_vf_;
            reset_thread_vf_.thread_F = thread_F.view();
            reset_thread_vf_.thread_V = &thread_V;

            reduce_vf_.thread_V = &thread_V;
            reduce_vf_.thread_F = thread_F.data();

            if (eval_chir_) {
                eval_chir_.value().V = &thread_V;
                eval_chir_.value().F = thread_F.data();
            }

            if (tethers_) {
                tethers_.value().V = &thread_V;
                tethers_.value().F = thread_F.data();
            }

            if (nat_ang_) {
                nat_ang_.value().V = &thread_V;
                nat_ang_.value().F = thread_F.data();
            }

            if (cnd_) {
                cnd_.value().V = &thread_V;
                cnd_.value().F = thread_F.data();
            }

            if (snd_) {
                snd_.value().V = &thread_V;
                snd_.value().F = thread_F.data();
            }

            if (eval_pauli_) {
                eval_pauli_.value().V = &thread_V;
                eval_pauli_.value().F = thread_F.data();
            }

            if (eval_go_) {
                eval_go_.value().V = &thread_V;
                eval_go_.value().F = thread_F.data();
            }

            if (eval_ss_) {
                eval_ss_.value().V = &thread_V;
                eval_ss_.value().F = thread_F.data();
            }

            if (eval_const_es_) {
                eval_const_es_.value().V = &thread_V;
                eval_const_es_.value().F = thread_F.data();
            }

            if (eval_rel_es_) {
                eval_rel_es_.value().V = &thread_V;
                eval_rel_es_.value().F = thread_F.data();
            }

            if (qa_proc_cont_) {
                qa_proc_cont_.value().V = &thread_V;
                qa_proc_cont_.value().F = thread_F.data();
            }

            if (eval_pid_) {
                eval_pid_.value().V = &thread_V;
                eval_pid_.value().F = thread_F.data();
            }

            if (eval_solid_) {
                eval_solid_.value().V = &thread_V;
                eval_solid_.value().F = thread_F.data();
            }

            if (eval_lj_attr_) {
                eval_lj_attr_.value().V = &thread_V;
                eval_lj_attr_.value().F = thread_F.data();
            }

            if (eval_vel_afm_) {
                eval_vel_afm_.value().V = &thread_V;
                eval_vel_afm_.value().F = thread_F.data();
            }

            if (eval_force_afm_) {
                eval_force_afm_.value().V = &thread_V;
                eval_force_afm_.value().F = thread_F.data();
            }

#pragma omp barrier

#pragma omp master
            {
                reset_vf_();
                verify_();
                qa_sift_cand_.omp_prep();

                if (nl_invalid) {
                    legacy_update_();
                    update_pauli_();
                    update_go_();
                    update_ss_();
                    update_const_es_();
                    qa_update_free_();
                    update_pid_();
                }
            };

#pragma omp barrier

            while (t < params.general.total_time) {
                reset_thread_vf_();
                eval_chir_.omp_async();
                tethers_.omp_async();
                nat_ang_.omp_async();
                cnd_.omp_async();
                snd_.omp_async();
                eval_pauli_.omp_async();
                eval_go_.omp_async();
                eval_ss_.omp_async();
                eval_const_es_.omp_async();
                eval_rel_es_.omp_async();
                precomp_nh_.omp_async();
                qa_sift_cand_.omp_async();
                qa_proc_cand_.omp_async();
                eval_pid_.omp_async();
                eval_solid_.omp_async();
                eval_lj_attr_.omp_async();
                eval_vel_afm_.omp_async();
                eval_force_afm_.omp_async();
                reduce_vf_();

#pragma omp barrier

#pragma omp master
                {
                    export_pdb_();
                    show_pbar_();
                    report_stats_();
                    rep_struct_();

                    lang_pc();
                    reset_vf_();
                    qa_sift_cand_.omp_prep();

                    verify_();
                    if (nl_invalid) {
                        legacy_update_();
                        update_pauli_();
                        update_go_();
                        update_ss_();
                        update_const_es_();
                        qa_update_free_();
                        update_pid_();
                    }
                };

#pragma omp barrier
            }
        }

        return EXIT_SUCCESS;
    }
}