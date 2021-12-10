#include "utils/units.h"
#include <cparse/shunting-yard.h>
#include <cparse/builtin-features.inc>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>

namespace xmd {
    quantity::quantity(true_real value):
        value {value} {}

    quantity::quantity(const std::string &s, quantity const& def_unit) {
        struct quantity_calculator_init {
            quantity_calculator_init() {
                using namespace cparse;
                cparse_startup();

                TokenMap& global = TokenMap::default_global();

                std::unordered_map<std::string, true_real> unit_map = {
                    { "f77unit", f77unit }, { "A", angstrom }, { "nm", nanometer },
                    { "m", meter }, { "ns", nanosecond }, { "tau", tau }, { "1/tau", 1/tau },
                    { "micros", microsecond }, { "ms", millisecond }, { "s", second },
                    { "atom", atom }, { "mol", mol }, { "eps", eps }, {"kcal", kcal},
                    { "J", Joule }, { "kB", kB }, { "K", Kelvin },
                    { "kg", kg }, { "amu", amu }, { "f77mass", f77mass }, { "e", echarge },
                    { "C", Coulomb }, { "Amp", Ampere }, { "c", cspeed }, { "H", Henry },
                    { "mu_0", mu_0 }, { "eps_0", eps_0 }, { "rad", rad }, { "deg", deg }
                };

                for (auto const& [name, val]: unit_map) {
                    global[name] = val;
                }
            }
        };
        static quantity_calculator_init init;

        auto space = std::find(s.begin(), s.end(), ' ');
        if (space == s.end()) {
            auto magnitude = calculator::calculate(s.c_str()).asDouble();
            value = magnitude * def_unit;
        }
        else {
            auto mag_str = std::string(s.begin(), space);
            auto mag = calculator::calculate(mag_str.c_str()).asDouble();

            auto unit_str = std::string(space + 1, s.end());
            auto unit_val = calculator::calculate(unit_str.c_str()).asDouble();

            value = mag * unit_val;
        }
    }

    quantity::operator true_real() const {
        return value;
    };

    quantity param_value_parser<quantity>::parse(
        const param_entry &entry) const {

        return quantity(entry.as<std::string>());
    }
}