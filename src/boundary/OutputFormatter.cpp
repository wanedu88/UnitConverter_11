#include "boundary/OutputFormatter.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace uc11 {

double roundHalfUp4(double value) {
    return std::round(value * 10000.0) / 10000.0;
}

std::string formatTableLine(double inputValue, const std::string& inputUnit, double convertedValue,
                            const std::string& targetUnit) {
    std::ostringstream left;
    left << inputValue << ' ' << inputUnit;

    std::ostringstream right;
    right << std::fixed << std::setprecision(4) << roundHalfUp4(convertedValue) << ' ' << targetUnit;

    return left.str() + " = " + right.str();
}

}  // namespace uc11
