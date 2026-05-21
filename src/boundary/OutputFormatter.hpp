#pragma once

#include "entity/UnitConverter.hpp"

#include <string>
#include <vector>

namespace uc11 {

double roundHalfUp4(double value);

std::string formatTableLine(double inputValue, const std::string& inputUnit, double convertedValue,
                            const std::string& targetUnit);

std::string formatJson(double inputValue, const std::string& inputUnit,
                       const std::vector<ConversionResult>& conversions);

}  // namespace uc11
