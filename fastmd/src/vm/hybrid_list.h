#pragma once
#include "list.h"
#include "alloc.h"
#include <type_traits>

namespace fastmd {
    namespace detail {
        template<typename T, typename OverflowAlloc>
        class def_hybrid_list_impl {
        public:
            using PrimaryList = fastmd::list<T, no_allocator<T>>;
            using OverflowList = fastmd::list<T, OverflowAlloc>;

            using size_type = std::common_type<
                typename PrimaryList::size_type,
                typename OverflowList::size_type>;

            ANY def_hybrid_list_impl();
            ANY explicit def_hybrid_list_impl(PrimaryList const& primary = PrimaryList(),
                OverflowList const& overflow = OverflowList());

            ANY size_type size() const;
            ANY PrimaryList const& primary() const;
            ANY OverflowList const& overflow() const;

            ANY T& operator[](size_type idx);

            ANY T& push_back(T const& x);

        private:
            PrimaryList _primary;
            OverflowList _overflow;
        };

        template<typename T, typename OverflowAlloc>
        struct hybrid_list_impl {
            using type = def_hybrid_list_impl<T, OverflowAlloc>;
        };
    }

    template<typename T, typename OverflowAlloc>
    using hybrid_list = typename detail::hybrid_list_impl<T, OverflowAlloc>::type;
}

#include "hybrid_list.inl"
