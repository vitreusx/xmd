#pragma once
#include <memory>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <mutex>
#include <xmd/utils/serialization.h>
#include <thread>

namespace xmd {
    class context;

    class ctx_aware {
    public:
        virtual ~ctx_aware() = default;
        virtual void declare_vars(context& ctx) = 0;
    };

    template <class T>
    constexpr std::string_view type_name()
    {
        using namespace std;
#ifdef __clang__
        string_view p = __PRETTY_FUNCTION__;
        return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
        string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
    }

    class ephemeral_var {
    public:
        inline bool has_value() const {
            return value_ptr != nullptr;
        }

        template<typename T>
        T& value() {
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

    class persistent_var {
    public:
        inline bool has_value() const {
            return value_ptr != nullptr;
        }

        template<typename T>
        void restore_as() {
            if (!has_value()) {
                std::string msg = "Persistent variable \"" + name + "\" hasn\'t been set or loaded from storage!";
                throw std::runtime_error(msg);
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

        std::string name;

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

    class context {
    public:
        bool has(std::string const& name) {
            return (eph_vars.find(name) != eph_vars.end()) ||
                   (per_vars.find(name) != per_vars.end());
        }

        template<typename T>
        T& var(std::string const& name) {
            std::lock_guard guard(access_mut);
            
            if (auto eph_iter = eph_vars.find(name); eph_iter != eph_vars.end()) {
                auto &eph_var = eph_iter->second;
                return eph_var.value<T>();
            } else if (auto per_iter = per_vars.find(name); per_iter != per_vars.end()) {
                auto &per_var = per_iter->second;
                return per_var.value<T>();
            } else {
                std::string msg =
                    "Context does not have a variable \"" + name + "\"";
                throw std::runtime_error(msg);
            }
        }

        template<typename T, typename... Args>
        T& ephemeral(std::string const& name, Args&&... args) {
            std::lock_guard guard(access_mut);

            if (auto iter = eph_vars.find(name); iter != eph_vars.end()) {
                auto &eph_var = iter->second;
                return eph_var.value<T>();
            }
            else {
                auto& eph_var = eph_vars[name];
                eph_var.set<T, Args...>(std::forward<Args>(args)...);

                auto& val = eph_var.value<T>();
                if constexpr (std::is_base_of_v<ctx_aware, T>) {
                    ((ctx_aware &) val).declare_vars(*this);
                }

                return val;
            }
        }

        template<typename T, typename... Args>
        T& persistent(std::string const& name, Args&&... args) {
            std::lock_guard guard(access_mut);

            if (auto iter = per_vars.find(name); iter != per_vars.end()) {
                auto &per_var = iter->second;
                if (!per_var.has_value())
                    per_var.restore_as<T>();
                return per_var.value<T>();
            }
            else {
                auto& per_var = per_vars[name];
                per_var.name = name;
                per_var.set<T>(std::forward<Args>(args)...);

                auto& val = per_var.value<T>();
                if constexpr (std::is_base_of_v<ctx_aware, T>) {
                    ((ctx_aware &) val).declare_vars(*this);
                }

                return val;
            }
        }

        context& per_thread();

    private:
        std::recursive_mutex access_mut;
        std::unordered_map<std::string, ephemeral_var> eph_vars;
        std::unordered_map<std::string, persistent_var> per_vars;
        std::unordered_map<std::thread::id, std::unique_ptr<context>> thread_contexts;

        friend class ::boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & per_vars;
        }
    };

    template<typename F>
    class lazy {
    public:
        explicit lazy(F&& f):
            f{std::forward<F>(f)} {};

        operator decltype(std::declval<F>()())() {
            return f();
        }

    private:
        F f;
    };
}

