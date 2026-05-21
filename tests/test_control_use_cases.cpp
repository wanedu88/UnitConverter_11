#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "control/LoadConfigUseCase.hpp"
#include "control/RegisterUnitUseCase.hpp"
#include "entity/ConversionConstants.hpp"
#include "entity/UnitConverter.hpp"

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

TEST_CASE("TC-C-02 LoadConfigUseCase missing file loads builtins", "[control][TC-C-02]") {
    const auto result =
        uc11::LoadConfigUseCase{}.execute(sourcePath("config/does_not_exist.json"));

    REQUIRE(result.ok);
    REQUIRE(result.errorMessage.empty());
    REQUIRE(result.registry.hasUnit("feet"));
    REQUIRE_THAT(uc11::convert(result.registry, "meter", 1.0, "feet"),
                 WithinAbs(uc11::kMeterToFeet, uc11::kRatioEpsilon));
}

TEST_CASE("TC-C-03 RegisterUnitUseCase registers cubit", "[control][TC-C-03]") {
    auto registry = uc11::UnitRegistry::withBuiltins();
    uc11::RegisterUnitUseCase{}.execute(registry, uc11::ConversionRule{"cubit", 0.4572});

    REQUIRE(registry.hasUnit("cubit"));
    REQUIRE(uc11::convert(registry, "cubit", 1.0, "meter") == Catch::Approx(0.4572).epsilon(1e-5));
}
