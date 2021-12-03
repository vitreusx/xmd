#include "files/param_file.h"
#include <yaml-cpp/yaml.h>
#include "files/csv.h"
#include <fstream>

namespace xmd {
    param_file::param_file(const std::filesystem::path &pf_path) {
        node = YAML::LoadFile(pf_path);
        pwd = pf_path.parent_path();

        if (auto inherit_node = node["inherit"]; inherit_node) {
            for (auto iter = inherit_node.begin(); iter != inherit_node.end(); ++iter) {
                auto path = pwd / iter->as<std::string>();
                inherited_pfs.emplace_back(path);
            }
        }
    }

    void param_file::load_entry(param_entry *entry) {
        for (auto& pf: inherited_pfs) {
            pf.load_entry(entry);
        }
        entry->load_from_node(node, pwd);
    }
}