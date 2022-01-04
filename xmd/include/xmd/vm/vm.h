#pragma once
#include <stdexcept>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <optional>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/traits.hpp>

namespace xmd {
    using vm_path = std::string;
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
        class resource {
        public:
            std::shared_ptr<void> value_ptr;

            using save_t = std::function<void(boost::archive::text_oarchive&, void*)>;
            std::optional<save_t> save;
        };

    private:

//        template<typename T, typename Function>
//        T& find_or_(_if_serializable const&, vm_path const& path, Function&& if_not_found) {
//            if (auto res_iter = resources.find(path); res_iter != resources.end()) {
//                resource& res = res_iter->second;
//                return *std::static_pointer_cast<T>(res.value_ptr).get();
//            }
//
//            if (auto loaded_iter = loaded.find(path); loaded_iter != loaded.end()) {
//                std::stringstream proxy;
//                proxy << loaded_iter->second;
//                boost::archive::text_iarchive proxy_ar(proxy);
//                T value;
//                proxy_ar & value;
//                return emplace<T>(path, std::move(value));
//            }
//
//            auto& ret = if_not_found();
//            return ret;
//        }

//        template<typename T, typename... Args>
//        T& emplace_(_if_serializable const&, vm_path const& path, Args&&... args) {
//            auto value = std::make_shared<T>(std::forward<Args>(args)...);
//
//            auto res = resource {
//                std::static_pointer_cast<void>(value),
//                [](auto& ar, void *vptr) -> void {
//                    T const& v = *(T const*)vptr;
//                    ar & v;
//                }
//            };
//
//            resources[path] = res;
//
//            auto& value_ref = find<T>(path);
//            if constexpr (std::is_base_of_v<vm_aware, T>)
//                ((vm_aware &)value_ref).init_from_vm(*this);
//
//            return find<T>(path);
//        }

    public:
        bool has(vm_path const& path) const;

        template<typename T, typename Function>
        T& find_or(vm_path const& path, Function&& if_not_found) {
            if (auto res_iter = resources.find(path); res_iter != resources.end()) {
                resource& res = res_iter->second;
                return *std::static_pointer_cast<T>(res.value_ptr).get();
            }

            auto& ret = if_not_found();
            return ret;
        }

        template<typename T>
        T& find(vm_path const& path) {
            return find_or<T>(path, [&path]() -> T& {
                throw resource_not_found(path);
            });
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

            auto res = resource {
                std::static_pointer_cast<void>(value),
                std::nullopt
            };

            resources[path] = res;

            auto& value_ref = find<T>(path);
            if constexpr (std::is_base_of_v<vm_aware, T>)
                ((vm_aware &)value_ref).init_from_vm(*this);

            return find<T>(path);
        };

        template<typename T, typename... Args>
        T& find_or_emplace(vm_path const& path, Args&&... args) {
            return find_or<T>(path, [&]() -> auto& {
                return emplace<T>(path, std::forward<Args>(args)...);
            });
        };

    private:
        std::unordered_map<vm_path, resource> resources;
        std::unordered_map<vm_path, std::string> loaded;

        friend class boost::serialization::access;

        template<class Archive>
        void save(Archive &ar, [[maybe_unused]] const unsigned int version) const {
            std::stringstream proxy {};
            boost::archive::text_oarchive proxy_ar(proxy);

            std::unordered_map<vm_path, std::string> serialized;
            for (auto const& [path, res]: resources) {
                if (res.save.has_value()) {
                    proxy.clear();
                    auto& save_func = res.save.value();
                    save_func(proxy_ar, res.value_ptr.get());
                    serialized[path] = proxy.str();
                }
            }

            ar & serialized;
        };

        template<class Archive>
        void load(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & loaded;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()
    };
}