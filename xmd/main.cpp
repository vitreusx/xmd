#include <xmd/files/pdb.h>
#include <random>
#include <xmd/utils/units.h>
#include <fstream>
#include <xmd/dynamics/leapfrog.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/forces/langevin.h>
#include <xmd/io/export_pdb.h>
#include <xmd/io/show_progress_bar.h>
using namespace xmd;

template<typename U>
v3::vec<U> cast_vec(Eigen::Vector3<U> const& v) {
    return { v.x(), v.y(), v.z() };
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

public:
    simulation(xmd::model const& xmd_model, amino_acid_data const& data,
            int seed) {
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

        for (int idx = 0; idx < num_particles; ++idx) {
            r[idx] = true_r[idx] = cast_vec(xmd_model.residues[idx]->pos);
            a_prev[idx] = F[idx] = v[idx] = true_v[idx] = vec3d::Zero();
            atype[idx] = xmd_model.residues[idx]->type;
            mass[idx] = (float)data[atype[idx]].mass;
            mass_inv[idx] = 1.0f / mass[idx];
            chain_idx[idx] = xmd_model.residues[idx]->parent->chain_idx;
            chain_seq_idx[idx] = xmd_model.residues[idx]->seq_idx;
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
        eval_langevin_dynamics<decltype(eng)> task(eng);
        task.num_particles = num_particles;
        task.mass = mass.to_array();
        task.temperature = temperature;
        task.gamma_factor = gamma_factor;
        task.v = v.to_array();
        task.F = F.to_array();
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

public:
    void operator()() {
        auto reset_vf_t = reset_vf_();
        auto perform_leapfog_step_t = perform_leapfrog_step_();
        auto eval_langevin_dynamics_t = eval_langevin_dynamics_();
        auto show_progress_bar_t = show_progress_bar_();
        auto export_pdb_t = export_pdb_();

        using namespace std::chrono;
        show_progress_bar_t.start_wall_time = high_resolution_clock::now();

        double pbar_update_period = 10.0*tau;
        double pbar_last_update_t = std::numeric_limits<double>::min();

        double pdb_export_period = 100.0*tau;
        double pdb_last_export_t = std::numeric_limits<double>::min();

        while (t < total_time) {
            reset_vf_t();
            eval_langevin_dynamics_t();
            perform_leapfog_step_t();

            if (t - pbar_last_update_t >= pbar_update_period) {
                show_progress_bar_t();
                pbar_last_update_t = t;
            }

            if (t - pdb_last_export_t >= pdb_export_period) {
                export_pdb_t();
                pdb_last_export_t = t;
            }
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