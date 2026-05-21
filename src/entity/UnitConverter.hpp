#pragma once

#include <string>
#include <utility>
#include <vector>

namespace uc11 {

class UnitRegistry;

struct ConversionResult {
    std::string unit;
    double value;
};

double convert(const UnitRegistry& registry, const std::string& fromUnit, double value,
               const std::string& toUnit);

std::vector<ConversionResult> convertAll(const UnitRegistry& registry, const std::string& fromUnit,
                                         double value);

}  // namespace uc11
