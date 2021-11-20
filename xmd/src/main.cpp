#include <iostream>
#include <fstream>
#include "files/param_file.h"

int main() {
    auto pf = xmd::param_file("data/params/MDCG/parameters.yml");

    return 0;
}
