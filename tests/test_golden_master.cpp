#include <catch2/catch_test_macros.hpp>

#include "TestPhase.hpp"
#include "golden_master_helper.hpp"

#include <filesystem>
#include <string>

#ifndef UC11_SOURCE_DIR
#define UC11_SOURCE_DIR "."
#endif

#ifndef UC11_LEGACY_EXE
#define UC11_LEGACY_EXE "UnitConverter"
#endif

namespace {

std::filesystem::path legacyExePath() {
#ifdef _WIN32
    return std::filesystem::path(UC11_LEGACY_EXE);
#else
    return std::filesystem::path(UC11_LEGACY_EXE);
#endif
}

std::filesystem::path expectedGoldenPath() {
    return std::filesystem::path(UC11_SOURCE_DIR) / "tests" / "golden_master_expected.txt";
}

}  // namespace

TEST_CASE("golden_master_cli_stdout_approval", "[integration][golden][REG-04]") {
    uc11_require_green_phase();

    const auto exe = legacyExePath();
    INFO("legacy executable: " << exe.string());
    REQUIRE(std::filesystem::exists(exe));

    const auto expectedPath = expectedGoldenPath();
    const std::string actual = uc11_golden::buildGoldenDocument(exe);

    if (!std::filesystem::exists(expectedPath)) {
        uc11_golden::writeFile(expectedPath, actual);
        FAIL("Golden master baseline created at " << expectedPath.string()
             << " — review, then git add tests/golden_master_expected.txt");
    }

    const std::string expected = uc11_golden::readFile(expectedPath);
    if (expected != actual) {
        const std::string diff = uc11_golden::diffLines(expected, actual);
        FAIL("Golden master mismatch\n" << diff);
    }
}
