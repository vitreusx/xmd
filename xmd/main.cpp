#include <xmd/files/pdb.h>
#include <random>
#include <xmd/dynamics/leapfrog.h>
#include <xmd/forces/langevin.h>
#include <xmd/utils/units.h>
#include <xmd/dynamics/reset_vf.h>
#include <xmd/dynamics/set_initial_vel.h>
#include <fstream>
using namespace xmd;

template<typename U>
v3::vec<U> cast_vec(Eigen::Vector3<U> const& v) {
    return { v.x(), v.y(), v.z() };
}

int main() {
    std::ifstream stream("data/models/1ubq.pdb");
    pdb pdb_file(stream);
    model model_ = pdb_file.to_model();

    int seed = 442;

    std::default_random_engine eng(seed);
    model_.morph_into_saw(eng, 3.8, 1e-3, false);

    int n = model_.residues.size();
    vec3f_vector r(n), v(n), F(n);
    for (int idx = 0; idx < n; ++idx) {
        r[idx] = cast_vec(model_.residues[idx]->pos);

    }

    vector<float> mass(n, 1.0*f77mass), mass_inv(n, 1.0/f77mass);
    float V, t=0.0f, temperature = 0.35f*eps/kB, dt=tau/200.0f;

    set_initial_vel<decltype(eng)> set_initial_vel_(eng);
    set_initial_vel_.v = v.to_array();
    set_initial_vel_.num_particles = n;
    set_initial_vel_.temperature = temperature;
    set_initial_vel_.mass_inv = mass_inv.to_array();

    reset_vf reset_vf_;
    reset_vf_.num_particles = n;
    reset_vf_.V = &V;
    reset_vf_.F = F.to_array();

    eval_langevin_dynamics<decltype(eng)> eval_langevin_dynamics_(eng);
    eval_langevin_dynamics_.num_particles = n;
    eval_langevin_dynamics_.gamma_factor = 2.0 / tau;
    eval_langevin_dynamics_.mass = mass.to_array();
    eval_langevin_dynamics_.temperature = temperature;
    eval_langevin_dynamics_.v = v.to_array();
    eval_langevin_dynamics_.F = F.to_array();

    perform_leapfrog_step perform_leapfrog_step_;
    perform_leapfrog_step_.num_particles = n;
    vec3d_vector a_prev(n);
    perform_leapfrog_step_.a_prev = a_prev.to_array();
    perform_leapfrog_step_.mass_inv = mass_inv.to_array();
    perform_leapfrog_step_.t = &t;
    perform_leapfrog_step_.v = v.to_array();
    perform_leapfrog_step_.F = F.to_array();
    perform_leapfrog_step_.dt = dt;
    perform_leapfrog_step_.r = r.to_array();

    set_initial_vel_();
    while (t < 15'000.0 *tau) {
        reset_vf_();
        eval_langevin_dynamics_();
        perform_leapfrog_step_();
    }

    return 0;
}