#pragma once

#include "entity/UnitConverter.hpp"

#include <string>

namespace uc11 {

struct ConvertLengthResult {
    std::string fromUnit;
    double inputValue;
    std::vector<ConversionResult> conversions;
};

class ConvertLengthUseCase {
public:
    ConvertLengthResult execute(const std::string& fromUnit, double value) const;
};

}  // namespace uc11
