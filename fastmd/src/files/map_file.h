#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace fastmd {
    class map_file {
    public:
        explicit map_file(std::filesystem::path const& path);

        struct contact {
            int i1, i2;
            float d;
        };
        std::vector<contact> contacts;

        struct angle {
            int i1, i2, i3;
            float theta;
        };
        std::vector<angle> angles;

        struct dihedral {
            int i1, i2, i3, i4;
            float phi;
        };
        std::vector<dihedral> dihedrals;
    };
}