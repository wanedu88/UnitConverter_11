#include "boundary/OutputFormatter.hpp"

#include "entity/UnitConverter.hpp"

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

std::string formatJson(double inputValue, const std::string& inputUnit,
                      const std::vector<ConversionResult>& conversions) {
    std::ostringstream json;
    json << std::fixed << std::setprecision(4);
    json << "{\"input\":{\"unit\":\"" << inputUnit << "\",\"value\":" << inputValue
         << "},\"conversions\":[";
    for (std::size_t i = 0; i < conversions.size(); ++i) {
        if (i > 0) {
            json << ',';
        }
        json << "{\"unit\":\"" << conversions[i].unit
             << "\",\"value\":" << roundHalfUp4(conversions[i].value) << '}';
    }
    json << "]}";
    return json.str();
}

}  // namespace uc11
