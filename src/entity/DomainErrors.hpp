#pragma once

#include <string>

namespace uc11 {
namespace DomainErr {

constexpr const char* kInvalidUnitNameEmpty = "Invalid unit name: (empty)";
constexpr const char* kFactorMustBePositive =
    "Unit registration failed: factor_to_meter must be positive";
constexpr const char* kDuplicateUnitPrefix = "Unit registration failed: duplicate unit ";
constexpr const char* kUnknownUnitPrefix = "Unknown unit: ";

inline std::string duplicateUnit(const std::string& name) {
    return std::string(kDuplicateUnitPrefix) + name;
}

inline std::string unknownUnit(const std::string& name) {
    return std::string(kUnknownUnitPrefix) + name;
}

}  // namespace DomainErr
}  // namespace uc11
