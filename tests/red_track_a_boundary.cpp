// Track A — UI / Boundary GREEN (docs/red_tests.md)
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "boundary/OutputFormatter.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#include <stdexcept>
#include <string>

using Catch::Matchers::WithinAbs;

TEST_CASE("test_parse_meter_25_returns_conversion_result", "[red][boundary][RED-A-01]") {
    const auto parsed = uc11::parseInputLine("meter:2.5");
    uc11::validateInput(parsed);
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, parsed.unit, parsed.value, "feet");
    REQUIRE(feet == Catch::Approx(8.20210).epsilon(1e-5));
}

TEST_CASE("test_parse_no_colon_throws_invalid_argument", "[red][boundary][RED-A-02]") {
    REQUIRE_THROWS_AS(uc11::parseInputLine("meter2.5"), std::invalid_argument);
}

TEST_CASE("test_validate_meter_neg10_throws_invalid_argument", "[red][boundary][RED-A-03]") {
    const auto parsed = uc11::parseInputLine("meter:-1.0");
    REQUIRE_THROWS_AS(uc11::validateInput(parsed), std::invalid_argument);
}

TEST_CASE("test_convert_parsec10_throws_unknown_unit", "[red][boundary][RED-A-04]") {
    const auto parsed = uc11::parseInputLine("parsec:1.0");
    uc11::validateInput(parsed);
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE_THROWS_AS(uc11::convert(registry, parsed.unit, parsed.value, "feet"),
                      std::invalid_argument);
}

TEST_CASE("test_format_table_preserves_input_meter_25", "[red][boundary][RED-A-05]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, "meter", 2.5, "feet");
    const std::string line = uc11::formatTableLine(2.5, "meter", feet, "feet");
    REQUIRE(line.find("2.5 meter") != std::string::npos);
    REQUIRE(line.find("= 8.2021 feet") != std::string::npos);
}

TEST_CASE("test_format_json_meter_25_valid_schema", "[red][boundary][RED-A-06]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const auto results = uc11::convertAll(registry, "meter", 2.5);
    const std::string json = uc11::formatJson(2.5, "meter", results);
    REQUIRE(json.find("\"unit\":\"meter\"") != std::string::npos);
    REQUIRE(json.find("\"value\":2.5") != std::string::npos);
    REQUIRE(json.find("\"conversions\":[") != std::string::npos);
    REQUIRE(json.find("\"unit\":\"feet\"") != std::string::npos);
    REQUIRE(json.find("8.2021") != std::string::npos);
}
