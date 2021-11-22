#include <xmd/files/pdb.h>
#include <xmd/files/seq_file.h>
#include <xmd/random/nr_device.h>
#include <fstream>

int main() {
    using namespace xmd;
    seq_file _9aac_seq_file("data/models/9aac.yml");
    auto _9aac_xmd_model = _9aac_seq_file.to_model();

    auto nr = nr_device(442);
    _9aac_xmd_model.morph_into_saw(nr, 3.8, 1e-4, true);

    pdb _9aac_pdb_model(_9aac_xmd_model);
    std::ofstream _9aac_pdb_file("9aac.pdb");
    _9aac_pdb_file << _9aac_pdb_model;

    return 0;
}