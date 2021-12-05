#include <iostream>
#include <xmd/meta/macros.h>

#define CONCAT_L_(acc, x) acc##x
#define CONCAT_L(...) FOLDL(CONCAT_L_, , __VA_ARGS__)

#define CONCAT_R_(x, acc) x##acc
#define CONCAT_R(...) FOLDR(CONCAT_R_, , __VA_ARGS__)

int main() {
    return 0;
}