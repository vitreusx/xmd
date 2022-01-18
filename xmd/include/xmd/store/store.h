#pragma once
#include <string>
#include <memory>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/optional.hpp>
#include <stdexcept>
#include <unordered_map>
#include <mutex>
#include <thread>

namespace xmd {
    class temp_var {
    public:
        bool has_value() const;

        template<typename T>
        T& value() {
            auto typeful_value_ptr = std::static_pointer_cast<T>(value_ptr);
            return *typeful_value_ptr;
        }

        template<typename T>
        T const& value() const {
            auto typeful_value_ptr = std::static_pointer_cast<T>(value_ptr);
            return *typeful_value_ptr;
        }

        template<typename T, typename... Args>
        void set(Args&&... args) {
            auto typeful_value_ptr = std::make_shared<T>(
                std::forward<Args>(args)...);
            value_ptr = std::static_pointer_cast<void>(typeful_value_ptr);
        }

    private:
        std::shared_ptr<void> value_ptr;
    };

    class saved_var {
    public:
        bool has_value() const;

        template<typename T>
        void restore() {
            if (!value_str.has_value()) {
                throw std::runtime_error("nothing to restore from");
            }
            else {

                std::stringstream proxy_ss;
                proxy_ss << value_str.value();

                proxy_iarchive proxy_ar(proxy_ss);
                if constexpr (std::is_default_constructible_v<T>) {
                    T value;
                    proxy_ar >> value;
                    auto typeful_value_ptr = std::make_shared<T>(value);
                    value_ptr = std::static_pointer_cast<void>(typeful_value_ptr);
                }
                else {
                    T *raw_ptr;
                    proxy_ar >> raw_ptr;
                    auto typeful_value_ptr = std::shared_ptr<T>(raw_ptr);
                    value_ptr = std::static_pointer_cast<void>(typeful_value_ptr);
                }
            }
        }

        template<typename T>
        T& value() {
            auto typeful_value_ptr = std::static_pointer_cast<T>(value_ptr);
            return *typeful_value_ptr;
        }

        template<typename T>
        T const& value() const {
            auto typeful_value_ptr = std::static_pointer_cast<T>(value_ptr);
            return *typeful_value_ptr;
        }

        template<typename T, typename... Args>
        void set(Args&&... args) {
            auto typeful_value_ptr = std::make_shared<T>(
                std::forward<Args>(args)...);

            value_ptr = std::static_pointer_cast<void>(typeful_value_ptr);

            std::stringstream proxy_ss;
            proxy_oarchive proxy_ar(proxy_ss);
            if constexpr (std::is_default_constructible_v<T>) {
                proxy_ar << *typeful_value_ptr;
            }
            else {
                proxy_ar << typeful_value_ptr.get();
            }
            value_str = proxy_ss.str();
        }

    private:
        using proxy_iarchive = ::boost::archive::text_iarchive;
        using proxy_oarchive = ::boost::archive::text_oarchive;

        std::shared_ptr<void> value_ptr;
        ::boost::optional<std::string> value_str;

        friend class ::boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & value_str;
        }
    };

    class store {
    public:
        bool has(std::string const& var) const;

        template<typename T>
        T& value_of(std::string const& var) {
            std::lock_guard guard(access_mut);

            if (auto temp_iter = temp_vars.find(var); temp_iter != temp_vars.end()) {
                auto& temp_var = temp_iter->second;
                return temp_var.template value<T>();
            }
            else if (auto saved_iter = saved_vars.find(var); saved_iter != saved_vars.end()) {
                auto& saved_var = saved_iter->second;
                return saved_var.template value<T>();
            }
        }

        template<typename T>
        T const& value_of(std::string const& var) const {
            std::lock_guard guard(access_mut);

            if (auto temp_iter = temp_vars.find(var); temp_iter != temp_vars.end()) {
                auto& temp_var = temp_iter->second;
                return temp_var.template value<T>();
            }
            else if (auto saved_iter = saved_vars.find(var); saved_iter != saved_vars.end()) {
                auto& saved_var = saved_iter->second;
                return saved_var.template value<T>();
            }
        }

        template<typename T, typename... Args>
        T& make_temp(std::string const& var, Args&&... args) {
            std::lock_guard guard(access_mut);

            if (auto iter = temp_vars.find(var); iter != temp_vars.end()) {
                throw std::runtime_error("temp var already exists");
            }
            else {
                auto& temp_var = temp_vars[var];
                temp_var.template set<T, Args...>(std::forward<Args>(args)...);

                auto& val = temp_var.template value<T>();
                return val;
            }
        }

        template<typename T, typename Init>
        T& make_saved(std::string const& var, Init&& init) {
            std::lock_guard guard(access_mut);

            if (auto iter = saved_vars.find(var); iter != saved_vars.end()) {
                auto& saved_var = iter->second;
                if (saved_var.has_value()) {
                    throw std::runtime_error("saved var already exists");
                }
                else {
                    saved_var.restore<T>();
                    return saved_var.template value<T>();
                }
            }
            else {
                auto& saved_var = saved_vars[var];
                saved_var.template set<T>(init());

                auto& val = saved_var.template value<T>();
                return val;
            }
        }

        store& per_thread();

    private:
        std::recursive_mutex access_mut;
        std::unordered_map<std::string, temp_var> temp_vars;
        std::unordered_map<std::string, saved_var> saved_vars;
        std::unordered_map<std::thread::id, std::unique_ptr<store>> thread_stores;

        friend class ::boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & saved_vars;
        }
    };
}