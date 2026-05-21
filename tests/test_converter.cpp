#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "entity/UnitConverter.hpp"
#include "entity/UnitRegistry.hpp"

TEST_CASE("TC-B-01 convert meter to feet within 1e-5", "[domain][TC-B-01]") {
    const uc11::UnitRegistry registry = uc11::UnitRegistry::withBuiltins();
    REQUIRE(uc11::convert(registry, "meter", 2.5, "feet") == Catch::Approx(8.20210).epsilon(1e-5));
}
