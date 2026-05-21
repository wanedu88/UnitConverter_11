// Track B — Domain / Logic GREEN (docs/red_tests.md)
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "boundary/ConfigLoader.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#ifndef UC11_SOURCE_DIR
#define UC11_SOURCE_DIR "."
#endif

namespace {

std::string sourcePath(const std::string& relative) {
    return std::string(UC11_SOURCE_DIR) + "/" + relative;
}

}  // namespace

TEST_CASE("test_convert_meter_to_feet_within_1e5", "[red][domain][RED-B-01]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "meter", 2.5, "feet") == Catch::Approx(8.20210).epsilon(1e-5));
}

TEST_CASE("test_convert_meter_to_yard_ratio_109361", "[red][domain][RED-B-02]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "meter", 1.0, "yard") ==
            Catch::Approx(uc11::kMeterToYard).epsilon(1e-4));
}

TEST_CASE("test_convert_all_meter10_returns_three_units", "[red][domain][RED-B-03]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const auto results = uc11::convertAll(registry, "meter", 1.0);
    REQUIRE(results.size() == 3);
}

TEST_CASE("test_register_cubit_then_convert_to_meter", "[red][domain][RED-B-04]") {
    auto registry = uc11::UnitRegistry::withBuiltins();
    registry.registerUnit("cubit", 0.4572);
    REQUIRE(uc11::convert(registry, "cubit", 1.0, "meter") == Catch::Approx(0.4572).epsilon(1e-5));
}

TEST_CASE("test_load_config_json_applies_feet_factor", "[red][domain][RED-B-05]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("config/units.json"), registry, error));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "feet") ==
            Catch::Approx(uc11::kMeterToFeet).epsilon(1e-4));
}

TEST_CASE("test_load_config_missing_keeps_builtin_ratios", "[red][domain][RED-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("config/does_not_exist.json"), registry, error));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "feet") ==
            Catch::Approx(uc11::kMeterToFeet).epsilon(1e-4));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "yard") ==
            Catch::Approx(uc11::kMeterToYard).epsilon(1e-4));
}
