#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "TestPhase.hpp"

#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "boundary/OutputFormatter.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#include <string>

using Catch::Matchers::WithinAbs;

TEST_CASE("test_parse_meter_25_happy_returns_dto", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: valid input meter:2.5
    const std::string input = "meter:2.5";

    // When: parse
    const uc11::ParsedInput parsed = uc11::parseInputLine(input);
    uc11::validateInput(parsed);

    // Then: unit and value extracted
    REQUIRE(parsed.unit == "meter");
    REQUIRE_THAT(parsed.value, WithinAbs(2.5, uc11::kConvertEpsilon));
}

TEST_CASE("test_happy_meter_25_convert_feet_returns", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: 1 meter = 3.28084 feet; parsed meter:2.5
    const auto registry = uc11::UnitRegistry::withBuiltins();
    const auto parsed = uc11::parseInputLine("meter:2.5");
    uc11::validateInput(parsed);

    // When: convert to feet
    const double feet = uc11::convert(registry, parsed.unit, parsed.value, "feet");

    // Then: 8.20210 within epsilon
    REQUIRE_THAT(feet, WithinAbs(8.20210, uc11::kConvertEpsilon));
}

TEST_CASE("test_format_preserves_input_left_meter_25", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: POL-OUT — left side preserves input; 1 meter = 3.28084 feet
    const auto registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, "meter", 2.5, "feet");

    // When: format table line
    const std::string line = uc11::formatTableLine(2.5, "meter", feet, "feet");

    // Then: contains original left pattern
    REQUIRE(line.find("2.5 meter") != std::string::npos);
    REQUIRE(line.find("= 8.2021 feet") != std::string::npos);
}

TEST_CASE("test_parse_meter_0_boundary_valid", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: zero allowed (POL-NEG-01)
    const auto parsed = uc11::parseInputLine("meter:0");

    // When: validate and convert
    uc11::validateInput(parsed);
    const auto registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, parsed.unit, parsed.value, "feet");

    // Then: zero feet
    REQUIRE_THAT(feet, WithinAbs(0.0, uc11::kConvertEpsilon));
}

TEST_CASE("test_validate_meter_neg10_throws", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: meter:-1.0
    const auto parsed = uc11::parseInputLine("meter:-1.0");

    // When / Then: negative rejected
    REQUIRE_THROWS_AS(uc11::validateInput(parsed), std::invalid_argument);
}

TEST_CASE("test_parse_parsec_unknown_on_convert", "[boundary][contract]") {
    uc11_require_green_phase();
    // Given: parsec:1.0 parses but unit unknown in registry
    const auto parsed = uc11::parseInputLine("parsec:1.0");
    uc11::validateInput(parsed);
    const auto registry = uc11::UnitRegistry::withBuiltins();

    // When / Then: convert throws
    REQUIRE_THROWS_AS(uc11::convert(registry, parsed.unit, parsed.value, "feet"),
                      std::invalid_argument);
}
