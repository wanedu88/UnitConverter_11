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

TEST_CASE("TC-B-07 loadConfig missing path keeps builtin ratios", "[domain][TC-B-07]") {
    uc11::UnitRegistry registry;
    std::string error;
    REQUIRE(uc11::loadConfig(sourcePath("config/does_not_exist.json"), registry, error));
    REQUIRE(registry.hasUnit("feet"));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "feet") == Catch::Approx(3.28084).epsilon(1e-5));
    REQUIRE(uc11::convert(registry, "meter", 1.0, "yard") == Catch::Approx(1.09361).epsilon(1e-5));
}
