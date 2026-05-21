#include "boundary/InputValidator.hpp"

#include "boundary/ErrorMessages.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace uc11 {

void validateInput(const ParsedInput& input) {
    if (std::isnan(input.value) || std::isinf(input.value)) {
        throw std::invalid_argument(ErrMsg::invalidNumber(input.value));
    }
    if (input.value < 0.0) {
        std::ostringstream oss;
        oss << ErrMsg::kNegativeValuePrefix << input.value;
        throw std::invalid_argument(oss.str());
    }
}

}  // namespace uc11
