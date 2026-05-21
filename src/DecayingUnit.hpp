#pragma once

#include "entity/UnitRegistry.hpp"

#include <string>
#include <vector>

namespace uc11 {

struct DecayingConversionResult {
    std::string unit;
    double value;
};

// Bonus track: dynamic unit registration with meter hub (RED — stub only).
// Must not modify legacy Item conversion table in UnitConverter.cpp.
class DecayingUnit {
public:
    DecayingUnit();

    void registerUnit(const std::string& name, double factorToMeter);

    double convert(const std::string& fromUnit, double value, const std::string& toUnit) const;

    std::vector<DecayingConversionResult> convertAll(const std::string& fromUnit, double value) const;

private:
    UnitRegistry registry_;
};

}  // namespace uc11
