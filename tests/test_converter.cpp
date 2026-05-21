#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

TEST_CASE("TC-B-01 convert meter to feet within 1e-5", "[domain][TC-B-01]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "meter", 2.5, "feet") == Catch::Approx(8.20210).epsilon(1e-5));
}

TEST_CASE("TC-B-02 convert meter to yard within 1e-5", "[domain][TC-B-02]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "meter", 1.0, "yard") == Catch::Approx(1.09361).epsilon(1e-5));
}

TEST_CASE("TC-B-03 convert feet to meter reverse within 1e-5", "[domain][TC-B-03]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "feet", 1.0, "meter") == Catch::Approx(0.30480).epsilon(1e-5));
}
