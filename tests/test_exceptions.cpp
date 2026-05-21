#include <catch2/catch_test_macros.hpp>

#include "TestPhase.hpp"

#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

#include <stdexcept>
#include <string>

namespace {

uc11::UnitRegistry builtinRegistry() { return uc11::UnitRegistry::withBuiltins(); }

}  // namespace

TEST_CASE("test_parse_no_colon_invalid_format", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: input without ':' separator
    const std::string input = "meter2.5";

    // When / Then: invalid format
    REQUIRE_THROWS_AS(uc11::parseInputLine(input), std::invalid_argument);
    try {
        uc11::parseInputLine(input);
    } catch (const std::invalid_argument& ex) {
        REQUIRE(std::string(ex.what()).find("Invalid format") != std::string::npos);
    }
}

TEST_CASE("test_parse_meter_abc_invalid_number", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: non-numeric value token
    const std::string input = "meter:abc";

    // When / Then: parse failure
    REQUIRE_THROWS_AS(uc11::parseInputLine(input), std::invalid_argument);
    try {
        uc11::parseInputLine(input);
    } catch (const std::invalid_argument& ex) {
        REQUIRE(std::string(ex.what()).find("Invalid number: abc") != std::string::npos);
    }
}

TEST_CASE("test_validate_meter_neg1_negative_not_allowed", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: parsed negative value (POL-NEG)
    uc11::ParsedInput input{"meter", -1.0};

    // When / Then: validation rejects
    REQUIRE_THROWS_AS(uc11::validateInput(input), std::invalid_argument);
    try {
        uc11::validateInput(input);
    } catch (const std::invalid_argument& ex) {
        REQUIRE(std::string(ex.what()).find("Negative value not allowed") != std::string::npos);
    }
}

TEST_CASE("test_convert_parsec_unknown_unit", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: 1 meter = 3.28084 feet; parsec not in registry
    const auto registry = builtinRegistry();

    // When / Then: unknown unit on convert
    REQUIRE_THROWS_AS(uc11::convert(registry, "parsec", 1.0, "feet"), std::invalid_argument);
    try {
        uc11::convert(registry, "parsec", 1.0, "feet");
    } catch (const std::invalid_argument& ex) {
        REQUIRE(std::string(ex.what()).find("Unknown unit: parsec") != std::string::npos);
    }
}

TEST_CASE("test_parse_feet_123_malformed_decimal", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: multiple decimal points in value
    const std::string input = "feet:1.2.3";

    // When / Then: invalid number
    REQUIRE_THROWS_AS(uc11::parseInputLine(input), std::invalid_argument);
}

TEST_CASE("test_register_duplicate_cubit_fails", "[boundary][exception]") {
    uc11_require_green_phase();
    // Given: cubit already registered
    auto registry = builtinRegistry();
    registry.registerUnit("cubit", 0.4572);

    // When / Then: duplicate registration fails
    REQUIRE_THROWS_AS(registry.registerUnit("cubit", 0.4572), std::invalid_argument);
}
