#include <iostream>
#include "univ/alloc.h"
#include "univ/tuple.h"
#include "univ/list.h"

int main() {
    using Ts = fastmd::tuple<int, double, float>;
    using Allocs = fastmd::detail::allocator_tuple<fastmd::host_allocator, Ts>;
    fastmd::detail::joint_list<Ts, Allocs> list;

    auto x = list.append({0, 1.0, 2.0f});
    std::cout << fastmd::nth<0>(x) << " " << fastmd::nth<1>(x) << " " << fastmd::nth<2>(x) << '\n';

    x = Ts(3, 4.0, 5.0f);
    std::cout << fastmd::nth<0>(x) << " " << fastmd::nth<1>(x) << " " << fastmd::nth<2>(x) << '\n';

    return EXIT_SUCCESS;
}
