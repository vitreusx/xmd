#include <fstream>
#include <iostream>
#include <xmd/files/seq_file.h>
#include <xmd/files/pdb.h>
#include <xmd/model/model.h>
#include <xmd/utils/units.h>
using namespace xmd;

int main() {
    seq_file glut_seq("data/models/glut.yml");
    auto xmd_glut = glut_seq.to_model();

    int seed = 2137;
    std::default_random_engine eng(seed);
    xmd_glut.morph_into_saw(eng, 3.8*angstrom, 1e-3*atom/pow(angstrom, 3.0),
        false);

    std::ofstream("glut.pdb") << pdb(xmd_glut);

    return EXIT_SUCCESS;
}
