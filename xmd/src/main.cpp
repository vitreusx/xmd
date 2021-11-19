#include <iostream>
#include <fstream>
#include "files/pdb.h"

int main() {
    std::ifstream _1ubq_pdb_file("data/models/1ubq.pdb");
    xmd::pdb _1ubq_pdb(_1ubq_pdb_file);

    std::ofstream saved_1ubq("1ubq.saved.pdb");
    saved_1ubq << _1ubq_pdb;

    return 0;
}
