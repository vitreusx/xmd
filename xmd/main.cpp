#include <xmd/files/pdb.h>
#include <xmd/random/nr_device.h>
#include <xmd/integrators/leapfrog.h>
#include <xmd/forces/langevin.h>
#include <xmd/utils/units.h>
#include <xmd/integrators/reset_vf.h>
#include <fstream>
using namespace xmd;

int main() {
    std::ifstream stream("data/models/1ubq.pdb");
    pdb pdb_file(stream);
    model model_ = pdb_file.to_model();

    int seed = 442;

    nr_device<Eigen::Vector3d> nr_eigen(seed);
    model_.morph_into_saw(nr_eigen, 3.8, 1e-3, false);

    int n = model_.residues.size();
    vec3f_vector r(n), v(n), F(n);
    vector<float> mass(n), mass_inv(n);
    float V, t, temperature, dt;

    nr_device<vec3f> nr_v3(seed);

    reset_vf rvf;
    rvf.num_particles = n;
    rvf.V = &V;
    rvf.F = F.to_array();

    add_langevin_dynamics<decltype(nr_v3)> lang_dyn(nr_v3);
    lang_dyn.num_particles = n;
    lang_dyn.gamma_factor = 2.0 / tau;
    lang_dyn.mass = mass.to_array();
    lang_dyn.temperature = temperature;
    lang_dyn.v = v.to_array();
    lang_dyn.F = F.to_array();

    perform_leapfrog_step leapfrog;
    leapfrog.num_particles = n;
    vec3f_vector a_prev(n);
    leapfrog.a_prev = a_prev.to_array();
    leapfrog.mass_inv = mass_inv.to_array();
    leapfrog.t = &t;
    leapfrog.v = v.to_array();
    leapfrog.F = F.to_array();
    leapfrog.dt = dt;
    leapfrog.r = r.to_array();

    while (t < 15'000.0 *tau) {
        rvf();
        lang_dyn();
        leapfrog();
    }

    return 0;
}