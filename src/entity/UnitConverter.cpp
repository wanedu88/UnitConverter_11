#include "entity/UnitConverter.hpp"

#include "entity/ConversionConstants.hpp"
#include "entity/UnitRegistry.hpp"

#include <algorithm>
#include <stdexcept>

namespace uc11 {

double convert(const UnitRegistry& registry, const std::string& fromUnit, double value,
               const std::string& toUnit) {
    if (!registry.hasUnit(fromUnit)) {
        throw std::invalid_argument("Unknown unit: " + fromUnit);
    }
    if (!registry.hasUnit(toUnit)) {
        throw std::invalid_argument("Unknown unit: " + toUnit);
    }
    if (fromUnit == "meter" && toUnit == "feet") {
        return value * kMeterToFeet;
    }
    if (fromUnit == "meter" && toUnit == "yard") {
        return value * kMeterToYard;
    }
    if (fromUnit == "feet" && toUnit == "meter") {
        return value * kFeetFactorToMeter;
    }
    throw std::invalid_argument("Conversion not implemented: " + fromUnit + " -> " + toUnit);
}

std::vector<ConversionResult> convertAll(const UnitRegistry& registry,
                                         const std::string& fromUnit, double value) {
    auto names = registry.listUnits();
    std::sort(names.begin(), names.end());

    std::vector<ConversionResult> results;
    results.reserve(names.size());
    for (const auto& target : names) {
        results.push_back({target, convert(registry, fromUnit, value, target)});
    }
    return results;
}

}  // namespace uc11
