#include "DecayingUnit.hpp"

#include "entity/UnitConverter.hpp"

namespace uc11 {

DecayingUnit::DecayingUnit() : registry_(UnitRegistry::withBuiltins()) {}

void DecayingUnit::registerUnit(const std::string& name, double factorToMeter) {
    registry_.registerUnit(name, factorToMeter);
}

double DecayingUnit::convert(const std::string& fromUnit, double value,
                             const std::string& toUnit) const {
    return uc11::convert(registry_, fromUnit, value, toUnit);
}

std::vector<DecayingConversionResult> DecayingUnit::convertAll(const std::string& fromUnit,
                                                               double value) const {
    const auto results = uc11::convertAll(registry_, fromUnit, value);
    std::vector<DecayingConversionResult> rows;
    rows.reserve(results.size());
    for (const auto& row : results) {
        rows.push_back({row.unit, row.value});
    }
    return rows;
}

}  // namespace uc11
