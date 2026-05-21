#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "boundary/ConfigLoader.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#include <string>

using Catch::Matchers::WithinAbs;

#ifndef UC11_SOURCE_DIR
#define UC11_SOURCE_DIR "."
#endif

namespace {

std::string sourcePath(const std::string& relative) {
    return std::string(UC11_SOURCE_DIR) + "/" + relative;
}

}  // namespace

TEST_CASE("test_load_json_valid_feet_factor", "[boundary][config]") {
    // Given: config/units.json with feet factor_to_meter 0.3048 (1 m = 3.28084 ft)
    uc11::UnitRegistry registry;
    std::string error;

    // When: load valid JSON
    const bool ok = uc11::loadConfig(sourcePath("config/units.json"), registry, error);

    // Then: load success and convert 1 m → feet
    REQUIRE(ok);
    REQUIRE(error.empty());
    const double feet = uc11::convert(registry, "meter", 1.0, "feet");
    REQUIRE_THAT(feet, WithinAbs(uc11::kMeterToFeet, uc11::kRatioEpsilon));
}

TEST_CASE("test_load_json_missing_file_defaults", "[boundary][config]") {
    // Given: non-existent path; expect builtin 3.28084 / 1.09361
    uc11::UnitRegistry registry;
    std::string error;

    // When: load missing file
    const bool ok = uc11::loadConfig(sourcePath("config/does_not_exist.json"), registry, error);

    // Then: defaults loaded, conversion works
    REQUIRE(ok);
    REQUIRE(registry.hasUnit("feet"));
    const double feet = uc11::convert(registry, "meter", 1.0, "feet");
    REQUIRE_THAT(feet, WithinAbs(3.28084, uc11::kRatioEpsilon));
    const double yard = uc11::convert(registry, "meter", 1.0, "yard");
    REQUIRE_THAT(yard, WithinAbs(1.09361, uc11::kRatioEpsilon));
}

TEST_CASE("test_load_yaml_valid_units", "[boundary][config]") {
    // Given: YAML fixture with same ratios
    uc11::UnitRegistry registry;
    std::string error;

    // When: load YAML
    const bool ok = uc11::loadConfig(sourcePath("tests/fixtures/units.yaml"), registry, error);

    // Then: three units and correct feet conversion
    REQUIRE(ok);
    REQUIRE(registry.size() == 3);
    REQUIRE_THAT(uc11::convert(registry, "meter", 2.5, "feet"), WithinAbs(8.20210, uc11::kConvertEpsilon));
}

TEST_CASE("test_load_json_invalid_syntax_fails", "[boundary][config]") {
    // Given: broken JSON file
    uc11::UnitRegistry registry;
    std::string error;

    // When: load bad.json
    const bool ok = uc11::loadConfig(sourcePath("tests/fixtures/bad.json"), registry, error);

    // Then: failure with message
    REQUIRE_FALSE(ok);
    REQUIRE(error.find("Config load failed") != std::string::npos);
}

TEST_CASE("test_load_json_zero_factor_fails", "[boundary][config]") {
    // Given: factor_to_meter = 0
    uc11::UnitRegistry registry;
    std::string error;

    // When: load zero_factor.json
    const bool ok = uc11::loadConfig(sourcePath("tests/fixtures/zero_factor.json"), registry, error);

    // Then: rejected
    REQUIRE_FALSE(ok);
    REQUIRE(error.find("factor_to_meter") != std::string::npos);
}

TEST_CASE("test_load_missing_then_meter_to_yard_still_works", "[boundary][config]") {
    // Given: missing config → defaults; 1 meter = 1.09361 yard
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("no/such/file.json"), registry, error));

    // When: convert to yard
    const double yard = uc11::convert(registry, "meter", 1.0, "yard");

    // Then: 1.09361
    REQUIRE_THAT(yard, WithinAbs(1.09361, uc11::kRatioEpsilon));
}
