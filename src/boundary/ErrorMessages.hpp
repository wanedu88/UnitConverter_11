#pragma once

#include <string>

namespace uc11 {
namespace ErrMsg {

constexpr const char* kInvalidFormat = "Invalid format. Use unit:value (ex: meter:2.5)";
constexpr const char* kInvalidUnitNameEmpty = "Invalid unit name: (empty)";
constexpr const char* kInputReadFailed = "Input read failed";
constexpr const char* kNegativeValuePrefix = "Negative value not allowed: ";

inline std::string invalidNumber(const std::string& token) {
    return "Invalid number: " + token;
}

inline std::string invalidNumber(double value) {
    return invalidNumber(std::to_string(value));
}

}  // namespace ErrMsg
}  // namespace uc11
