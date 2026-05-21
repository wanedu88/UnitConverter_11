#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "TestPhase.hpp"

#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

using Catch::Matchers::WithinAbs;

namespace {

uc11::UnitRegistry builtinRegistry() { return uc11::UnitRegistry::withBuiltins(); }

}  // namespace

TEST_CASE("test_meter_to_feet_value_25_returns_82021", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: 1 meter = 3.28084 feet
    const auto registry = builtinRegistry();

    // When: convert 2.5 meter to feet
    const double result = uc11::convert(registry, "meter", 2.5, "feet");

    // Then: 2.5 * 3.28084 = 8.2021 (raw); within 1e-5 of 8.20210
    REQUIRE_THAT(result, WithinAbs(8.20210, uc11::kConvertEpsilon));
}

TEST_CASE("test_meter_to_yard_value_1_returns_109361", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: 1 meter = 1.09361 yard
    const auto registry = builtinRegistry();

    // When: convert 1 meter to yard
    const double result = uc11::convert(registry, "meter", 1.0, "yard");

    // Then: exact ratio within epsilon
    REQUIRE_THAT(result, WithinAbs(1.09361, uc11::kRatioEpsilon));
}

TEST_CASE("test_feet_to_meter_value_1_returns_030480", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: 1 feet = 0.3048 meter (1/3.28084); inverse of 1 meter = 3.28084 feet
    const auto registry = builtinRegistry();

    // When: reverse convert 1 feet to meter
    const double result = uc11::convert(registry, "feet", 1.0, "meter");

    // Then: 0.30480 within 1e-5
    REQUIRE_THAT(result, WithinAbs(0.30480, uc11::kConvertEpsilon));
}

TEST_CASE("test_meter_to_feet_value_1_matches_ratio_328084", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: 1 meter = 3.28084 feet (golden ratio)
    const auto registry = builtinRegistry();

    // When: 1 meter → feet
    const double result = uc11::convert(registry, "meter", 1.0, "feet");

    // Then: 3.28084 ± 1e-4
    REQUIRE_THAT(result, WithinAbs(uc11::kMeterToFeet, uc11::kRatioEpsilon));
}

TEST_CASE("test_meter_to_yard_value_25_returns_27340", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: 1 meter = 1.09361 yard
    const auto registry = builtinRegistry();

    // When: 2.5 meter → yard
    const double result = uc11::convert(registry, "meter", 2.5, "yard");

    // Then: 2.5 * 1.09361 = 2.734025
    REQUIRE_THAT(result, WithinAbs(2.734025, uc11::kConvertEpsilon));
}

TEST_CASE("test_feet_to_yard_via_meter_matches_indirect", "[entity][normal]") {
    uc11_require_green_phase();
    // Given: feet↔yard only via meter hub; 1 meter = 3.28084 feet, 1 meter = 1.09361 yard
    const auto registry = builtinRegistry();

    // When: 3.28084 feet → yard (≈ 1 meter)
    const double result = uc11::convert(registry, "feet", 3.28084, "yard");

    // Then: ≈ 1.09361 yard
    REQUIRE_THAT(result, WithinAbs(1.09361, 1e-3));
}
