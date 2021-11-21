#include <iostream>
#include <fstream>
#include "files/seq_file.h"
#include "files/pdb.h"
#include "random/nr_device.h"

int main() {
    auto nr = xmd::nr_device(442);

    auto glut_sf = xmd::seq_file("data/models/glut.yml");
    auto glut_xmd_model = glut_sf.to_model();
    glut_xmd_model.morph_into_saw(nr, 3.8, 4.56, 1e-4, true);
    auto glut_pdb_model = xmd::pdb(glut_xmd_model);

    std::ofstream glut_pdb_file("glut.pdb");
    glut_pdb_file << glut_pdb_model;

    return 0;
}
