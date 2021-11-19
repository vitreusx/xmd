#include "pdb.h"
#include "detail/fields.h"
#include "detail/records.h"
#include <sstream>
#include <string_view>

namespace xmd {
    pdb::pdb(std::istream &is) {
        std::unordered_map<char, bool> ter_found;

        for (std::string line; std::getline(is, line); ) {

        }
    }
}