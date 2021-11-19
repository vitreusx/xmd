#include <iostream>
#include <fstream>
#include "files/seq_file.h"

int main() {
    auto glut_sf = xmd::seq_file("data/models/glut.yml");
    auto const& glut_m = glut_sf.to_model();

    return 0;
}
