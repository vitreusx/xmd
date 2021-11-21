#include <iostream>
#include <fstream>
//#include "files/param_file.h"
#include "random/nr_device.h"

int main() {
    auto nr = xmd::nr_device(448);
    for (size_t i = 0; i < 10; ++i)
        std::cout << nr.normal() << '\n';

    return 0;
}
