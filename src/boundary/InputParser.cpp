#include "boundary/InputParser.hpp"

#include "boundary/ErrorMessages.hpp"

#include <cstddef>
#include <stdexcept>

namespace uc11 {

ParsedInput parseInputLine(const std::string& line) {
    const std::size_t pos = line.find(':');
    if (pos == std::string::npos) {
        throw std::invalid_argument(ErrMsg::kInvalidFormat);
    }

    ParsedInput result;
    result.unit = line.substr(0, pos);
    const std::string valueStr = line.substr(pos + 1);

    if (result.unit.empty()) {
        throw std::invalid_argument(ErrMsg::kInvalidUnitNameEmpty);
    }

    try {
        std::size_t consumed = 0;
        result.value = std::stod(valueStr, &consumed);
        if (consumed != valueStr.size()) {
            throw std::invalid_argument(ErrMsg::invalidNumber(valueStr));
        }
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument(ErrMsg::invalidNumber(valueStr));
    } catch (const std::out_of_range&) {
        throw std::invalid_argument(ErrMsg::invalidNumber(valueStr));
    }

    return result;
}

}  // namespace uc11
