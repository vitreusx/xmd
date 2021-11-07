#pragma once

namespace fastmd::detail {
    template<typename T, typename OverflowAlloc>
    ANY def_hybrid_list_impl<T, OverflowAlloc>::def_hybrid_list_impl():
        _primary{PrimaryList()}, _overflow{OverflowList()} {}

    template<typename T, typename OverflowAlloc>
    ANY def_hybrid_list_impl<T, OverflowAlloc>::def_hybrid_list_impl(
        const PrimaryList &primary, const OverflowList &overflow):
        _primary{primary}, _overflow{overflow} {}

    template<typename T, typename OverflowAlloc>
    ANY typename def_hybrid_list_impl<T, OverflowAlloc>::size_type
    def_hybrid_list_impl<T, OverflowAlloc>::size() const {
        return (size_type)_primary.size() + (size_type)_overflow.size();
    }

    template<typename T, typename OverflowAlloc>
    ANY typename def_hybrid_list_impl<T, OverflowAlloc>::PrimaryList const&
    def_hybrid_list_impl<T, OverflowAlloc>::primary() const {
        return _primary;
    }

    template<typename T, typename OverflowAlloc>
    ANY typename def_hybrid_list_impl<T, OverflowAlloc>::OverflowList const&
    def_hybrid_list_impl<T, OverflowAlloc>::overflow() const {
        return _primary;
    }

    template<typename T, typename OverflowAlloc>
    ANY T& def_hybrid_list_impl<T, OverflowAlloc>::operator[](size_type idx) {
        if (idx < _primary.size()) return _primary[idx];
        else return _overflow[idx - _primary.size()];
    }

    template<typename T, typename OverflowAlloc>
    ANY T& def_hybrid_list_impl<T, OverflowAlloc>::push_back(const T &x) {
        if (_primary.size() < _primary.capacity()) {
            return _primary.push_back(x);
        }
        else {
            return _overflow.push_back(x);
        }
    }
}