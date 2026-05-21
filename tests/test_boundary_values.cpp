#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#include <cmath>
#include <limits>

using Catch::Matchers::WithinAbs;

namespace {

uc11::UnitRegistry builtinRegistry() { return uc11::UnitRegistry::withBuiltins(); }

}  // namespace

TEST_CASE("test_meter_to_feet_value_0_returns_0", "[entity][boundary]") {
    // Given: 1 meter = 3.28084 feet; value = 0 (POL-NEG zero allowed)
    const auto registry = builtinRegistry();

    // When: convert 0 meter to feet
    const double result = uc11::convert(registry, "meter", 0.0, "feet");

    // Then: 0 feet
    REQUIRE_THAT(result, WithinAbs(0.0, uc11::kConvertEpsilon));
}

TEST_CASE("test_meter_to_yard_value_0_returns_0", "[entity][boundary]") {
    // Given: 1 meter = 1.09361 yard; value = 0
    const auto registry = builtinRegistry();

    // When: convert 0 meter to yard
    const double result = uc11::convert(registry, "meter", 0.0, "yard");

    // Then: 0 yard
    REQUIRE_THAT(result, WithinAbs(0.0, uc11::kConvertEpsilon));
}

TEST_CASE("test_meter_to_feet_large_value_1e100_finite", "[entity][boundary]") {
    // Given: 1 meter = 3.28084 feet; very large input
    const auto registry = builtinRegistry();
    constexpr double large = 1e100;

    // When: convert large meter value
    const double result = uc11::convert(registry, "meter", large, "feet");

    // Then: finite product (no overflow to inf)
    REQUIRE(std::isfinite(result));
    REQUIRE_THAT(result, WithinAbs(large * uc11::kMeterToFeet, large * uc11::kMeterToFeet * 1e-9));
}

TEST_CASE("test_meter_to_feet_precision_6decimals", "[entity][boundary]") {
    // Given: 1 meter = 3.28084 feet; input with 6 decimal places
    const auto registry = builtinRegistry();
    constexpr double input = 1.123456;

    // When: convert with 6-decimal input
    const double result = uc11::convert(registry, "meter", input, "feet");
    const double expected = input * uc11::kMeterToFeet;

    // Then: preserve precision within 1e-5
    REQUIRE_THAT(result, WithinAbs(expected, uc11::kConvertEpsilon));
}

TEST_CASE("test_convert_all_meter_zero_all_zero", "[entity][boundary]") {
    // Given: builtin registry; value = 0
    const auto registry = builtinRegistry();

    // When: convertAll from meter
    const auto results = uc11::convertAll(registry, "meter", 0.0);

    // Then: every target is 0
    REQUIRE(results.size() == 3);
    for (const auto& row : results) {
        REQUIRE_THAT(row.value, WithinAbs(0.0, uc11::kConvertEpsilon));
    }
}

TEST_CASE("test_meter_to_feet_value_000001_micro", "[entity][boundary]") {
    // Given: 1 meter = 3.28084 feet; tiny positive value
    const auto registry = builtinRegistry();
    constexpr double tiny = 1e-6;

    // When: micro value conversion
    const double result = uc11::convert(registry, "meter", tiny, "feet");

    // Then: tiny * 3.28084
    REQUIRE_THAT(result, WithinAbs(tiny * uc11::kMeterToFeet, uc11::kConvertEpsilon));
}
