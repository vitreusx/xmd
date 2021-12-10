#pragma once
#include <string>
#include <typeinfo>
#include <sstream>

namespace xmd {
    class vm_path {
    public:
        vm_path();
        vm_path(char const* s);
        vm_path(std::string s);

        template<typename T>
        static vm_path of_type() {
            return { std::string(typeid(T).name()) };
        }

        template<typename T>
        static vm_path of_inst(T& x) {
            std::stringstream ss;
            ss << std::hex << &x;
            return { ss.str() };
        }

        vm_path operator/(vm_path const& other) const;

        bool operator==(vm_path const& other) const;
        bool operator!=(vm_path const& other) const;

        std::string repr;
        friend std::hash<vm_path>;
    };
}

namespace std {
    template<>
    struct hash<xmd::vm_path> {
        size_t operator()(xmd::vm_path const& p) const {
            return hash<string>()(p.repr);
        }
    };
}