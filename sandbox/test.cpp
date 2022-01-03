#include <iostream>
#include <gentypes/gentype.h>

#define NAMESPACE(...) __VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() example
#define FIELDS() int,p,float,q,char,r

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

template<typename E>
std::ostream& operator<<(std::ostream& os, example_expr<E> const& e) {
    os << "example(" << e.p() << ", " << e.q() << ", " << e.r() << ")";
    return os;
}

template<typename E>
std::ostream& operator<<(std::ostream& os, set_node_expr<E> const& e) {
    os << "set_node(" << e.value() << ", " << e.vacant() << ")";
    return os;
}

template<typename NodeAlloc, typename Idx>
std::ostream& operator<<(std::ostream& os, example_set<NodeAlloc, Idx> const& set_) {
    os << "{";
    bool first = true;
    for (Idx i = 0; i < set_.extent(); ++i) {
        auto node = set_.at(i);
        if (node.vacant()) continue;
        auto value = node.value();

        if (!first) os << ", ";
        os << value;
        first = false;
    }
    os << "}";

    return os;
}

template<typename NodeAlloc, typename Idx>
std::ostream& operator<<(std::ostream& os, example_vector<NodeAlloc, Idx> const& vec_) {
    os << "[";
    bool first = true;
    for (Idx i = 0; i < vec_.size(); ++i) {
        if (!first) os << ", ";
        os << vec_[i];
        first = false;
    }
    os << "]";

    return os;
}

template<typename T, typename NodeAlloc, typename Idx>
std::ostream& operator<<(std::ostream& os, set_node_vector<T, NodeAlloc, Idx> const& vec_) {
    os << "[";
    bool first = true;
    for (Idx i = 0; i < vec_.size(); ++i) {
        if (!first) os << ", ";
        os << vec_[i];
        first = false;
    }
    os << "]";

    return os;
}

int main() {
    set<example> S;
    for (int i = 0; i < 8; ++i) {
        S.emplace(i, (float)(-i), (char)('0'+i));
    }
    std::cout << S << '\n';

    return 0;
}