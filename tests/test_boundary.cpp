#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "boundary/InputParser.hpp"
#include "boundary/InputValidator.hpp"
#include "boundary/OutputFormatter.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("TC-A-01 parse meter 2.5 happy path returns conversion", "[boundary][TC-A-01]") {
    const uc11::ParsedInput parsed = uc11::parseInputLine("meter:2.5");
    uc11::validateInput(parsed);
    REQUIRE(parsed.unit == "meter");
    REQUIRE_THAT(parsed.value, WithinAbs(2.5, uc11::kConvertEpsilon));

    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, parsed.unit, parsed.value, "feet");
    REQUIRE(feet == Catch::Approx(8.20210).epsilon(1e-5));
}

TEST_CASE("TC-A-06 format preserves input unit and value", "[boundary][TC-A-06]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, "meter", 2.5, "feet");
    const std::string line = uc11::formatTableLine(2.5, "meter", feet, "feet");
    REQUIRE(line.find("2.5 meter") != std::string::npos);
    REQUIRE(line.find("= 8.2021 feet") != std::string::npos);
}

TEST_CASE("TC-A-07 value zero boundary converts to zero feet", "[boundary][TC-A-07]") {
    const uc11::ParsedInput parsed = uc11::parseInputLine("meter:0");
    uc11::validateInput(parsed);
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    const double feet = uc11::convert(registry, parsed.unit, parsed.value, "feet");
    REQUIRE_THAT(feet, WithinAbs(0.0, uc11::kConvertEpsilon));
}
