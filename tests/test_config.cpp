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

TEST_CASE("TC-B-06 loadConfig valid path applies feet ratio", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("config/units.json"), registry, error));
    REQUIRE(error.empty());
    REQUIRE(uc11::convert(registry, "meter", 1.0, "feet") ==
            Catch::Approx(uc11::kMeterToFeet).epsilon(1e-5));
}

TEST_CASE("TC-B-05b registerUnit validation errors", "[domain][TC-B-05]") {
    auto registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE_THROWS_AS(registry.registerUnit("", 1.0), std::invalid_argument);
    REQUIRE_THROWS_AS(registry.registerUnit("inch", 0.0), std::invalid_argument);
}

TEST_CASE("TC-B-06b loadConfig malformed json fails", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE_FALSE(uc11::loadConfig(sourcePath("tests/fixtures/bad.json"), registry, error));
    REQUIRE(error.find("Config load failed") != std::string::npos);
}

TEST_CASE("TC-B-06e loadConfig missing factor field fails", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE_FALSE(
        uc11::loadConfig(sourcePath("tests/fixtures/missing_factor_field.json"), registry, error));
    REQUIRE(error.find("missing factor_to_meter") != std::string::npos);
}

TEST_CASE("TC-B-06f loadConfig malformed name fails", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE_FALSE(uc11::loadConfig(sourcePath("tests/fixtures/malformed_name.json"), registry, error));
    REQUIRE(error.find("malformed name") != std::string::npos);
}

TEST_CASE("TC-B-06d loadConfig empty units fails", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE_FALSE(uc11::loadConfig(sourcePath("tests/fixtures/empty_units.json"), registry, error));
    REQUIRE(error.find("no units defined") != std::string::npos);
}

TEST_CASE("TC-B-06c loadConfig yaml fixture", "[domain][TC-B-06]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("tests/fixtures/units.yaml"), registry, error));
    REQUIRE(registry.size() == 3);
}

TEST_CASE("TC-B-07 loadConfig missing path keeps builtin ratios", "[domain][TC-B-07]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("config/does_not_exist.json"), registry, error));
    REQUIRE(registry.hasUnit("feet"));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "feet") ==
            Catch::Approx(uc11::kMeterToFeet).epsilon(1e-5));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "yard") ==
            Catch::Approx(uc11::kMeterToYard).epsilon(1e-5));
}
