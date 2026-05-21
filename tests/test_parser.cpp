#include <catch2/catch_test_macros.hpp>

#include "boundary/InputParser.hpp"

#include <stdexcept>
#include <string>

TEST_CASE("TC-A-02 parse missing colon throws invalid_argument", "[boundary][TC-A-02]") {
    REQUIRE_THROWS_AS(uc11::parseInputLine("meter2.5"), std::invalid_argument);
    try {
        uc11::parseInputLine("meter2.5");
    } catch (const std::invalid_argument& ex) {
        REQUIRE(std::string(ex.what()).find("Invalid format") != std::string::npos);
    }
}
