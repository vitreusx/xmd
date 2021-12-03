#include <random>
#include <fstream>
#include <xmd/files/pdb.h>
#include <xmd/utils/units.h>
#include <xmd/dynamics/leapfrog.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/forces/langevin.h>
#include <xmd/io/export_pdb.h>
#include <xmd/io/show_progress_bar.h>
#include <xmd/forces/tether.h>
#include <xmd/forces/angle/native.h>
#include <xmd/forces/dihedral/complex_native.h>

using namespace xmd;

template<typename U>
v3::vec<U> cast_vec(Eigen::Vector3<U> const& v) {
    return { v.x(), v.y(), v.z() };
};

class tether_pair_vector {
public:
    explicit tether_pair_vector(int n = 0):
        i1{n}, i2{n}, nat_dist{n}, size{n} {};

    auto to_span() {
        tether_pair_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.nat_dist = nat_dist.to_array();
        span.size = size;
        return span;
    }

    vector<int> i1, i2;
    vector<float> nat_dist;
    int size;
};

class native_angle_vector {
public:
    explicit native_angle_vector(int n = 0):
        i1{n}, i2{n}, i3{n}, nat_theta{n}, size{n} {};

    auto to_span() {
        native_angle_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.nat_theta = nat_theta.to_array();
        span.size = size;
        return span;
    }

    vector<int> i1, i2, i3;
    vector<float> nat_theta;
    int size;
};

class native_dihedral_vector {
public:
    explicit native_dihedral_vector(int n = 0):
        i1{n}, i2{n}, i3{n}, i4{n}, nat_phi{n}, size{n} {};

    auto to_span() {
        native_dihedral_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.i3 = i3.to_array();
        span.i4 = i4.to_array();
        span.nat_phi = nat_phi.to_array();
        span.size = size;
        return span;
    }

    vector<int> i1, i2, i3, i4;
    vector<float> nat_phi;
    int size;
};

class simulation {
public:
    double dt;
    float temperature, gamma_factor;
    float total_time;

private:
    std::default_random_engine eng;
    vec3f_vector r, v, F;
    vec3d_vector true_r, true_v, a_prev;
    vector<float> mass, mass_inv;
    vector<amino_acid> atype;
    vector<int8_t> chain_idx;
    vector<int> chain_seq_idx;
    float V, t;
    double true_t;
    int num_particles, num_chains;
    std::ofstream output_pdb;
    int pdb_serial;
    tether_pair_vector tethers;
    native_angle_vector angles;
    native_dihedral_vector dihedrals;
    xorshift64 lang_xorshift;

public:
    simulation(xmd::model const& xmd_model, amino_acid_data const& data,
            uint64_t seed): lang_xorshift(seed) {
        eng = std::default_random_engine(seed);

        num_particles = (int)xmd_model.residues.size();
        num_chains = (int)xmd_model.chains.size();
        r = v = F = vec3f_vector(num_particles);
        true_r = true_v = a_prev = vec3d_vector(num_particles);
        mass = mass_inv = vector<float>(num_particles);
        atype = vector<amino_acid>(num_particles);
        chain_idx = vector<int8_t>(num_particles);
        chain_seq_idx = vector<int>(num_particles);
        true_t = 0.0;
        V = t = 0.0f;
        output_pdb = std::ofstream("output.pdb");
        pdb_serial = 0;

        std::unordered_map<xmd::model::residue const*, int> res_ptr_to_idx;
        for (int idx = 0; idx < num_particles; ++idx) {
            res_ptr_to_idx[xmd_model.residues[idx].get()] = idx;
            r[idx] = true_r[idx] = cast_vec(xmd_model.residues[idx]->pos);
            a_prev[idx] = F[idx] = v[idx] = true_v[idx] = vec3d::Zero();
            atype[idx] = xmd_model.residues[idx]->type;
            mass[idx] = (float)data[atype[idx]].mass;
            mass_inv[idx] = 1.0f / mass[idx];
            chain_idx[idx] = (int8_t)xmd_model.residues[idx]->parent->chain_idx;
            chain_seq_idx[idx] = xmd_model.residues[idx]->seq_idx;
        }

        auto xmd_tethers = xmd_model.tethers();
        tethers = tether_pair_vector((int)xmd_tethers.size());
        for (int idx = 0; idx < tethers.size; ++idx) {
            auto& tether_ = xmd_tethers[idx];
            tethers.i1[idx] = res_ptr_to_idx[tether_.res1];
            tethers.i2[idx] = res_ptr_to_idx[tether_.res2];
            tethers.nat_dist[idx] = (float)tether_.length;
        }

        angles = native_angle_vector((int)xmd_model.angles.size());
        for (int idx = 0; idx < angles.size; ++idx) {
            auto& angle_ = xmd_model.angles[idx];
            angles.i1[idx] = res_ptr_to_idx[angle_.res1];
            angles.i2[idx] = res_ptr_to_idx[angle_.res2];
            angles.i3[idx] = res_ptr_to_idx[angle_.res3];
            angles.nat_theta[idx] = (float)angle_.theta;
        }

        dihedrals = native_dihedral_vector((int)xmd_model.dihedrals.size());
        for (int idx = 0; idx < dihedrals.size; ++idx) {
            auto& dihedral_ = xmd_model.dihedrals[idx];
            dihedrals.i1[idx] = res_ptr_to_idx[dihedral_.res1];
            dihedrals.i2[idx] = res_ptr_to_idx[dihedral_.res2];
            dihedrals.i3[idx] = res_ptr_to_idx[dihedral_.res3];
            dihedrals.i4[idx] = res_ptr_to_idx[dihedral_.res4];
            dihedrals.nat_phi[idx] = (float)dihedral_.phi;
        }
    }

public:
    auto perform_leapfrog_step_() {
        perform_leapfrog_step task;
        task.mass_inv = mass_inv.to_array();
        task.num_particles = num_particles;
        task.v = v.to_array();
        task.true_v = true_v.to_array();
        task.a_prev = a_prev.to_array();
        task.t = &t;
        task.true_t = &true_t;
        task.r = r.to_array();
        task.true_r = true_r.to_array();
        task.dt = dt;
        task.F = F.to_array();
        return task;
    }

    auto reset_vf_() {
        reset_vf task;
        task.num_particles = num_particles;
        task.V = &V;
        task.F = F.to_array();
        return task;
    }

    auto eval_langevin_dynamics_() {
        eval_langevin_dynamics task;
        task.num_particles = num_particles;
        task.mass = mass.to_array();
        task.temperature = temperature;
        task.gamma_factor = gamma_factor;
        task.v = v.to_array();
        task.F = F.to_array();
        task.gen = &lang_xorshift;
        return task;
    }

    auto export_pdb_() {
        export_pdb task(output_pdb);
        task.chain_idx = chain_idx.to_array();
        task.num_chains = num_chains;
        task.chain_seq_idx = chain_seq_idx.to_array();
        task.num_particles = num_particles;
        task.atype = atype.to_array();
        task.serial = &pdb_serial;
        task.true_r = true_r.to_array();
        return task;
    }

    auto show_progress_bar_() {
        show_progress_bar task;
        task.total_time = total_time;
        task.V = &V;
        task.width = 50;
        task.true_t = &true_t;
        return task;
    }

    auto eval_tether_forces_() {
        eval_tether_forces task;
        task.H1 = 100.0*eps/(angstrom*angstrom);
        task.H2 = 0.0;
        task.r = r.to_array();
        task.F = F.to_array();
        task.V = &V;
        task.tethers = tethers.to_span();
        return task;
    }

    auto eval_native_angle_forces_() {
        eval_native_angle_forces task;
        task.k = 30.0 * eps/(rad*rad);
        task.angles = angles.to_span();
        task.V = &V;
        task.F = F.to_array();
        task.r = r.to_array();
        return task;
    }

    auto eval_cnd_forces_() {
        eval_cnd_forces task;
        task.dihedrals = dihedrals.to_span();
        task.V = &V;
        task.CDA = 0.66*eps/(rad*rad);
        task.CDB = 0.66*eps/(rad*rad);
        task.F = F.to_array();
        task.r = r.to_array();
        return task;
    }

public:
    void operator()() {
        auto reset_vf_t = reset_vf_();
        auto perform_leapfog_step_t = perform_leapfrog_step_();
        auto eval_langevin_dynamics_t = eval_langevin_dynamics_();
        auto show_progress_bar_t = show_progress_bar_();
        auto export_pdb_t = export_pdb_();
        auto eval_tether_forces_t = eval_tether_forces_();
        auto eval_native_angle_forces_t = eval_native_angle_forces_();
        auto eval_cnd_forces_t = eval_cnd_forces_();

        using namespace std::chrono;
        show_progress_bar_t.start_wall_time = high_resolution_clock::now();

        double pbar_update_period = 10.0*tau;
        double pbar_last_update_t = std::numeric_limits<double>::lowest();

        double pdb_export_period = 100.0*tau;
        double pdb_last_export_t = std::numeric_limits<double>::lowest();

        while (t < total_time) {
            reset_vf_t();
            eval_langevin_dynamics_t();
            eval_tether_forces_t();
            eval_native_angle_forces_t();
            eval_cnd_forces_t();

            if (t - pbar_last_update_t >= pbar_update_period) {
                show_progress_bar_t();
                pbar_last_update_t = t;
            }

            if (t - pdb_last_export_t >= pdb_export_period) {
                export_pdb_t();
                pdb_last_export_t = t;
            }

            perform_leapfog_step_t();
        }
    }
};

int main() {
    std::ifstream stream("data/models/1ubq.pdb");
    pdb pdb_model(stream);

    amino_acid_data data;
    data.load_from_file("data/params/defaults/amino-acids.yml");
    pdb_model.add_contacts(data, true);

    model xmd_model = pdb_model.to_model();

    int seed = 442;
    std::default_random_engine eng(seed);
    xmd_model.morph_into_saw(eng, 3.8*angstrom, 1e-3*atom/pow(angstrom, 3.0),
        false);

    simulation simulation_(xmd_model, data, seed);
    simulation_.dt = 5e-3 * tau;
    simulation_.total_time = 15e3 * tau;
    simulation_.temperature = 0.35*eps/kB;
    simulation_.gamma_factor = 2.0f/tau;

    simulation_();

    std::ofstream("morphed.pdb") << pdb(xmd_model);
    return 0;
}