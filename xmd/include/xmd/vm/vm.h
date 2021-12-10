#pragma once
#include <memory>
#include <unordered_map>
#include "path.h"

namespace xmd {
    class vm;

    class vm_aware {
    public:
        virtual ~vm_aware() = default;
        virtual void init_from_vm(vm& vm_inst) = 0;
    };

    class resource_not_found: public std::logic_error {
    public:
        explicit resource_not_found(vm_path const& path);
    };

    class vm {
    public:
        template<typename T, typename Function>
        T& find_or(vm_path const& path, Function&& if_not_found) {
            auto iter = resources.find(path);
            if (iter != resources.end()) {
                return *std::static_pointer_cast<T>(iter->second).get();
            }
            else {
                auto& ret = if_not_found();
                return ret;
            }
        }

        template<typename T>
        T& find(vm_path const& path) {
            auto iter = resources.find(path);
            if (iter != resources.end()) {
                return *std::static_pointer_cast<T>(resources[path]).get();
            }
            else {
                throw resource_not_found(path);
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
            auto value = std::make_shared<T>(std::forward<Args>(args)...);
            auto as_void_ptr = std::static_pointer_cast<void>(value);
            resources[path] = std::move(value);
            auto& resource = find<T>(path);
            if constexpr (std::is_base_of_v<vm_aware, T>)
                ((vm_aware &) resource).init_from_vm(*this);
            return find<T>(path);
        };

        template<typename T, typename... Args>
        T& find_or_emplace(vm_path const& path, Args&&... args) {
            return find_or<T>(path, [&]() -> auto& {
                return emplace<T>(path, std::forward<Args>(args)...);
            });
        }

    private:
        std::unordered_map<vm_path, std::shared_ptr<void>> resources;
    };
}