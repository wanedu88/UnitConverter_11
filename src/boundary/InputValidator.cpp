#include "boundary/InputValidator.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace uc11 {

void validateInput(const ParsedInput& input) {
    if (std::isnan(input.value) || std::isinf(input.value)) {
        throw std::invalid_argument("Invalid number: " + std::to_string(input.value));
    }
    if (input.value < 0.0) {
        std::ostringstream oss;
        oss << "Negative value not allowed: " << input.value;
        throw std::invalid_argument(oss.str());
    }
}

}  // namespace uc11
