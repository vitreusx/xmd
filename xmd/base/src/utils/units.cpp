#include "utils/units.h"
#include <algorithm>
#include <unordered_map>
#include <stdexcept>

namespace xmd {
    double parse_qty(std::string const& quantity, double def_unit) {
        auto space = std::find(quantity.begin(), quantity.end(), ' ');
        if (space == quantity.end()) {
            return std::stod(quantity) * def_unit;
        }
        else {
            auto value = std::stod(std::string(quantity.begin(), space));
            auto unit_str = std::string(space + 1, quantity.end());

            static std::unordered_map<std::string, double> unit_map = {
                { "f77unit", f77unit }, { "A", angstrom }, { "nm", nanometer },
                { "m", meter }, { "ns", nanosecond }, { "tau", tau },
                { "micros", microsecond }, { "ms", millisecond }, { "s", second },
                { "atom", atom }, { "mol", mol }, { "eps", eps }, {"kcal", kcal},
                { "J", Joule }, { "eps/kB", eps_kB }, { "kB", kB }, { "K", Kelvin },
                { "kg", kg }, { "amu", amu }, { "f77mass", f77mass }, { "e", echarge },
                { "C", Coulomb }, { "Amp", Ampere }, { "c", cspeed }, { "H", Henry },
                { "mu_0", mu_0 }, { "eps_0", eps_0 }, { "rad", rad }, { "deg", deg }
            };

            if (auto iter = unit_map.find(unit_str); iter != unit_map.end()) {
                return value * iter->second;
            }
            else {
                throw std::runtime_error("unit not found!");
            }
        }
    }
}