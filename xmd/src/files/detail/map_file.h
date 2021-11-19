#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace xmd {
    class map_file {
    public:
        struct contact {
            int i1, i2;
            double length;
        };
        std::vector<contact> contacts;

        struct angle {
            int i1, i2, i3;
            double theta;
        };
        std::vector<angle> angles;

        struct dihedral {
            int i1, i2, i3, i4;
            double phi;
        };
        std::vector<dihedral> dihedrals;
    };
}

namespace YAML {
    template<>
    struct convert<xmd::map_file> {
        static Node encode(const xmd::map_file& mf);
        static bool decode(const Node& node, xmd::map_file& mf);
    };
}