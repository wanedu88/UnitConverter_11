#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "control/ConvertLengthUseCase.hpp"
#include "entity/ConversionConstants.hpp"

TEST_CASE("TC-C-01 ConvertLengthUseCase meter 2.5 returns three conversions", "[control][TC-C-01]") {
    const uc11::ConvertLengthResult result =
        uc11::ConvertLengthUseCase{}.execute("meter", 2.5);

    REQUIRE(result.fromUnit == "meter");
    REQUIRE(result.inputValue == Catch::Approx(2.5));
    REQUIRE(result.conversions.size() == 3);

    for (const auto& row : result.conversions) {
        if (row.unit == "feet") {
            REQUIRE(row.value == Catch::Approx(8.20210).epsilon(1e-5));
        } else if (row.unit == "yard") {
            REQUIRE(row.value == Catch::Approx(2.5 * uc11::kMeterToYard).epsilon(1e-5));
        } else if (row.unit == "meter") {
            REQUIRE(row.value == Catch::Approx(2.5).epsilon(1e-5));
        }
    }
}
