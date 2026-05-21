#pragma once

#include <string>

namespace uc11 {

double roundHalfUp4(double value);

std::string formatTableLine(double inputValue, const std::string& inputUnit, double convertedValue,
                            const std::string& targetUnit);

}  // namespace uc11
