#include "types/vec3.h"
#include <utils/units.h>

namespace YAML {
    Node convert<xmd::vec3r>::encode(const xmd::vec3r& v) {
        std::vector<xmd::real> values = { v.x(), v.y(), v.z() };
        return convert<decltype(values)>::encode(values);
    }

    bool convert<xmd::vec3r>::decode(const Node &node, xmd::vec3r &v) {
        std::vector<std::string> values;
        if (!convert<decltype(values)>::decode(node, values))
            return false;

        v.x() = xmd::quantity(values[0]);
        v.y() = xmd::quantity(values[1]);
        v.z() = xmd::quantity(values[2]);
        return true;
    }
}