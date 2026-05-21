#pragma once

namespace uc11 {
namespace ConfigErr {

constexpr const char* kMissingFactor = "Config load failed: missing factor_to_meter";
constexpr const char* kInvalidFactor = "Config load failed: invalid factor_to_meter";
constexpr const char* kMalformedName = "Config load failed: malformed name";
constexpr const char* kFactorMustBePositive =
    "Config load failed: factor_to_meter must be positive";
constexpr const char* kNoUnitsDefined = "Config load failed: no units defined";

}  // namespace ConfigErr
}  // namespace uc11
