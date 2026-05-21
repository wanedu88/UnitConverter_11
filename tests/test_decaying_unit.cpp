#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "DecayingUnit.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr double kCubitToMeter = 0.4572;
constexpr double kMeterToFeet = 3.28084;
constexpr double kBonusEpsilon = 1e-5;

bool containsUnit(const std::vector<uc11::DecayingConversionResult>& rows, const std::string& unit) {
    for (const auto& row : rows) {
        if (row.unit == unit) {
            return true;
        }
    }
    return false;
}

}  // namespace

TEST_CASE("감쇠 단위 변환", "[bonus]") {
    SECTION("[BT-01] registerUnit cubit then cubit to meter") {
        uc11::DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);
        REQUIRE(converter.convert("cubit", 1.0, "meter") == Catch::Approx(kCubitToMeter).epsilon(kBonusEpsilon));
    }

    SECTION("[BT-02] registerUnit cubit then meter to cubit inverse") {
        uc11::DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);
        const double expected = 1.0 / kCubitToMeter;
        REQUIRE(converter.convert("meter", 1.0, "cubit") == Catch::Approx(expected).epsilon(kBonusEpsilon));
    }

    SECTION("[BT-03] registerUnit cubit then cubit to feet cross conversion") {
        uc11::DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);
        const double expected = kCubitToMeter * kMeterToFeet;
        REQUIRE(converter.convert("cubit", 1.0, "feet") == Catch::Approx(expected).epsilon(kBonusEpsilon));
    }

    SECTION("[BT-04] negative factor registration throws invalid_argument") {
        uc11::DecayingUnit converter;
        REQUIRE_THROWS_AS(converter.registerUnit("badunit", -0.4572), std::invalid_argument);
    }

    SECTION("[BT-05] registerUnit cubit then convertAll returns every unit") {
        uc11::DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);
        const auto results = converter.convertAll("cubit", 1.0);
        REQUIRE(results.size() == 4);
        REQUIRE(containsUnit(results, "meter"));
        REQUIRE(containsUnit(results, "feet"));
        REQUIRE(containsUnit(results, "yard"));
        REQUIRE(containsUnit(results, "cubit"));
    }

    SECTION("[BT-06] builtin meter to feet unchanged after cubit registration") {
        uc11::DecayingUnit converter;
        converter.registerUnit("cubit", kCubitToMeter);
        REQUIRE(converter.convert("meter", 1.0, "feet") == Catch::Approx(kMeterToFeet).epsilon(kBonusEpsilon));
    }
}
