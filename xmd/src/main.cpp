#include <iostream>
#include <fstream>
#include "files/pdb.h"

int main() {
    std::ifstream _1ubq_pdb_file("data/models/1ubq.pdb");
    xmd::pdb _1ubq_pdb(_1ubq_pdb_file);

    return 0;
}
