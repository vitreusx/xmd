#include "config.h"
#include "univ/list.h"
#include "univ/alloc.h"

__device__ void f() {
    using Ts = fastmd::tuple<int, double, float>;
    using Allocs = fastmd::detail::allocator_tuple<fastmd::device_allocator, Ts>;
    fastmd::detail::joint_list<Ts, Allocs> list;

    auto x = list.append(Ts(0, 1.0, 2.0f));

    printf("%d\n", fastmd::nth<0>(x));

    x = Ts(3, 4.0, 5.0f);
    printf("%d\n", fastmd::nth<0>(x));
}
