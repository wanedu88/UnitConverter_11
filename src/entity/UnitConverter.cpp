#include "entity/UnitConverter.hpp"

#include "entity/ConversionConstants.hpp"
#include "entity/UnitRegistry.hpp"

#include <algorithm>
#include <stdexcept>

namespace uc11 {

double convert(const UnitRegistry& registry, const std::string& fromUnit, double value,
               const std::string& toUnit) {
    const double meters = value * registry.factorToMeter(fromUnit);
    return meters / registry.factorToMeter(toUnit);
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
