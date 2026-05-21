#include "boundary/InputParser.hpp"

#include <cstddef>
#include <stdexcept>

namespace uc11 {

ParsedInput parseInputLine(const std::string& line) {
    const std::size_t pos = line.find(':');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid format. Use unit:value (ex: meter:2.5)");
    }

    ParsedInput result;
    result.unit = line.substr(0, pos);
    const std::string valueStr = line.substr(pos + 1);

    if (result.unit.empty()) {
        throw std::invalid_argument("Invalid unit name: (empty)");
    }

    try {
        std::size_t consumed = 0;
        result.value = std::stod(valueStr, &consumed);
        if (consumed != valueStr.size()) {
            throw std::invalid_argument("Invalid number: " + valueStr);
        }
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid number: " + valueStr);
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Invalid number: " + valueStr);
    }

    return result;
}

}  // namespace uc11
