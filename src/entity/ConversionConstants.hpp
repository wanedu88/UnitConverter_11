#pragma once

namespace uc11 {

// 1 meter = 3.28084 feet; 1 meter = 1.09361 yard (PRD builtin)
inline constexpr double kMeterToFeet = 3.28084;
inline constexpr double kMeterToYard = 1.09361;
inline constexpr double kFeetFactorToMeter = 1.0 / kMeterToFeet;  // 0.3048
inline constexpr double kYardFactorToMeter = 1.0 / kMeterToYard;  // 0.9144

inline constexpr double kConvertEpsilon = 1e-5;
inline constexpr double kRatioEpsilon = 1e-4;

}  // namespace uc11
