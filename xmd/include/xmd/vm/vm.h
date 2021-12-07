#pragma once
#include <memory>
#include <unordered_map>
#include <any>
#include "path.h"

namespace xmd {
    class vm;

    class vm_aware {
    public:
        virtual void bind_to_vm(vm& vm_inst) = 0;
    };

    class vm {
    public:
        template<typename T>
        T& find(vm_path const& path) {
            return std::any_cast<T&>(resources.at(path));
        }

        template<typename T, typename Function>
        T& find_or(vm_path const& path, Function&& if_not_found) {
            auto iter = resources.find(path);
            if (iter != resources.end()) {
                return std::any_cast<T&>(iter->second);
            }
            else {
                auto& ret = if_not_found();
                return ret;
            }
        }

        template<typename T>
        T& add(vm_path const& path, T const& value) {
            return emplace<T>(path, value);
        }

        template<typename T>
        T& find_or_add(vm_path const& path, T const& value) {
            return find_or<T>(path, [&]() -> auto& {
                return add<T>(path, value);
            });
        }

        template<typename T, typename... Args>
        T& emplace(vm_path const& path, Args&&... args) {
            auto value = std::make_any<T>(std::forward<Args>(args)...);
            auto& resource = (resources[path] = std::move(value));
            if constexpr (std::is_base_of_v<vm_aware, T>)
                ((vm_aware&)resource).bind_to_vm(*this);
            return std::any_cast<T&>(resource);
        };

        template<typename T, typename... Args>
        T& find_or_emplace(vm_path const& path, Args&&... args) {
            return find_or<T>(path, [&]() -> auto& {
                return emplace<T>(path, std::forward<Args>(args)...);
            });
        }

    private:
        std::unordered_map<vm_path, std::any> resources;
    };
}