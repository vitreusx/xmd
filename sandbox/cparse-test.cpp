#include <cparse/shunting-yard.h>
#include <cparse/builtin-features.inc>
#include <iostream>

int main() {
    using namespace cparse;
    cparse_startup();

    TokenMap vars;
    vars["pi"] = 3.14;
    std::cout << calculator::calculate("-pi+1", &vars) << '\n';
    return 0;
}