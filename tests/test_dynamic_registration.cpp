#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("test_register_cubit_then_meter_convert", "[entity][dynamic]") {
    // Given: 1 cubit = 0.4572 meter (dynamic registration)
    auto registry = uc11::UnitRegistry::withBuiltins();
    registry.registerUnit("cubit", 0.4572);

    // When: 1 cubit → meter
    const double result = uc11::convert(registry, "cubit", 1.0, "meter");

    // Then: 0.4572 m
    REQUIRE_THAT(result, WithinAbs(0.4572, uc11::kConvertEpsilon));
}

TEST_CASE("test_register_cubit_convertall_includes_cubit", "[entity][dynamic]") {
    // Given: cubit registered; 1 meter = 3.28084 feet unchanged
    auto registry = uc11::UnitRegistry::withBuiltins();
    registry.registerUnit("cubit", 0.4572);

    // When: convertAll 1 meter
    const auto results = uc11::convertAll(registry, "meter", 1.0);

    // Then: 4 units including cubit
    REQUIRE(results.size() == 4);
    bool foundCubit = false;
    for (const auto& row : results) {
        if (row.unit == "cubit") {
            foundCubit = true;
            REQUIRE_THAT(row.value, WithinAbs(1.0 / 0.4572, 1e-3));
        }
    }
    REQUIRE(foundCubit);
}

TEST_CASE("test_register_two_units_increases_count", "[entity][dynamic]") {
    // Given: empty beyond builtins
    auto registry = uc11::UnitRegistry::withBuiltins();
    const auto before = registry.size();

    // When: register inch and cubit
    registry.registerUnit("inch", 0.0254);
    registry.registerUnit("cubit", 0.4572);

    // Then: size +2
    REQUIRE(registry.size() == before + 2);
}

TEST_CASE("test_register_cubit_factor_exact_ratio", "[entity][dynamic]") {
    // Given: cubit factor 0.4572 meter per cubit
    auto registry = uc11::UnitRegistry::withBuiltins();
    registry.registerUnit("cubit", 0.4572);

    // When: cubit → feet via meter hub (1 m = 3.28084 ft)
    const double feet = uc11::convert(registry, "cubit", 1.0, "feet");
    const double expected = 0.4572 * uc11::kMeterToFeet;

    // Then: hub conversion matches
    REQUIRE_THAT(feet, WithinAbs(expected, uc11::kConvertEpsilon));
}

TEST_CASE("test_register_invalid_zero_factor_throws", "[entity][dynamic]") {
    // Given: invalid factor
    auto registry = uc11::UnitRegistry::withBuiltins();

    // When / Then: registration rejected
    REQUIRE_THROWS_AS(registry.registerUnit("badunit", 0.0), std::invalid_argument);
}

TEST_CASE("test_register_cubit_meter_to_feet_golden_unchanged", "[entity][dynamic]") {
    // Given: cubit added; builtin 1 meter = 3.28084 feet
    auto registry = uc11::UnitRegistry::withBuiltins();
    registry.registerUnit("cubit", 0.4572);

    // When: meter:2.5 equivalent convert
    const double feet = uc11::convert(registry, "meter", 2.5, "feet");

    // Then: golden 8.20210 still holds
    REQUIRE_THAT(feet, WithinAbs(8.20210, uc11::kConvertEpsilon));
}
