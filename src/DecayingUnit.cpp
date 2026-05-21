#include "DecayingUnit.hpp"

namespace uc11 {

DecayingUnit::DecayingUnit() = default;

void DecayingUnit::registerUnit(const std::string& /*name*/, double /*factorToMeter*/) {
    // RED stub — GREEN will validate factor > 0 and store in registry.
}

double DecayingUnit::convert(const std::string& /*fromUnit*/, double /*value*/,
                             const std::string& /*toUnit*/) const {
    // RED stub — GREEN will use meter hub without touching legacy Item table.
    return 0.0;
}

std::vector<DecayingConversionResult> DecayingUnit::convertAll(const std::string& /*fromUnit*/,
                                                               double /*value*/) const {
    // RED stub — GREEN will return one row per registered unit.
    return {};
}

}  // namespace uc11
