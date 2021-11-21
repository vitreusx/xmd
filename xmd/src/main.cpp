#include <iostream>
#include <fstream>
#include "files/pdb.h"
#include "model/model.h"

int main() {
    std::ifstream _1ubq_pdb_file("data/models/1ubq.pdb");
    xmd::pdb _1ubq_pdb_model(_1ubq_pdb_file);
    xmd::model _1ubq_xmd_model = _1ubq_pdb_model.to_model();
    return 0;
}
