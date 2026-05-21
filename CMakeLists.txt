cmake_minimum_required(VERSION 3.16)

if(WIN32 AND NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(_UC11_GXX NAMES g++ g++.exe)
    if(_UC11_GXX)
        set(CMAKE_CXX_COMPILER "${_UC11_GXX}" CACHE FILEPATH "" FORCE)
    endif()
endif()

project(UnitConverter_11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(UC11_GREEN_PHASE "TDD GREEN: enable real assertions in unit_converter_tests" ON)
option(UC11_COVERAGE "Enable gcov coverage" OFF)

function(uc11_enable_coverage target_name)
    if(UC11_COVERAGE)
        target_compile_options(${target_name} PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(${target_name} PRIVATE --coverage)
    endif()
endfunction()

add_library(uc11_entity
    src/entity/UnitRegistry.cpp
    src/entity/UnitConverter.cpp
)
target_include_directories(uc11_entity PUBLIC ${CMAKE_SOURCE_DIR}/src)
uc11_enable_coverage(uc11_entity)

add_library(uc11_data
    src/data/ConfigLoader.cpp
)
target_include_directories(uc11_data PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(uc11_data PUBLIC uc11_entity)
uc11_enable_coverage(uc11_data)

add_library(uc11_control
    src/control/ConvertLengthUseCase.cpp
    src/control/LoadConfigUseCase.cpp
    src/control/RegisterUnitUseCase.cpp
)
target_include_directories(uc11_control PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(uc11_control PUBLIC uc11_entity uc11_data)
uc11_enable_coverage(uc11_control)

add_library(uc11_boundary
    src/boundary/CliApp.cpp
    src/boundary/InputParser.cpp
    src/boundary/InputValidator.cpp
    src/boundary/OutputFormatter.cpp
)
target_include_directories(uc11_boundary PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(uc11_boundary PUBLIC uc11_entity uc11_control)
uc11_enable_coverage(uc11_boundary)

add_executable(unit_converter_legacy UnitConverter.cpp)
set_target_properties(unit_converter_legacy PROPERTIES OUTPUT_NAME UnitConverter)
target_link_libraries(unit_converter_legacy PRIVATE uc11_boundary)
uc11_enable_coverage(unit_converter_legacy)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_converter_tests
    tests/test_normal_conversion.cpp
    tests/test_boundary_values.cpp
    tests/test_exceptions.cpp
    tests/test_dynamic_registration.cpp
    tests/test_config_load.cpp
    tests/test_boundary_contract.cpp
    tests/test_golden_master.cpp
)
target_link_libraries(unit_converter_tests PRIVATE uc11_entity uc11_boundary uc11_data
    Catch2::Catch2WithMain)
add_dependencies(unit_converter_tests unit_converter_legacy)
uc11_enable_coverage(unit_converter_tests)
target_include_directories(unit_converter_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests)
target_compile_definitions(unit_converter_tests PRIVATE
    UC11_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    UC11_LEGACY_EXE="$<TARGET_FILE:unit_converter_legacy>"
)
if(UC11_GREEN_PHASE)
    target_compile_definitions(unit_converter_tests PRIVATE UC11_GREEN_PHASE=1)
endif()

add_executable(unit_converter_red_tests
    tests/red_track_a_boundary.cpp
    tests/red_track_b_domain.cpp
    tests/test_converter.cpp
    tests/test_parser.cpp
    tests/test_boundary.cpp
    tests/test_config.cpp
    tests/test_control_convert_length.cpp
    tests/test_control_use_cases.cpp
)
target_link_libraries(unit_converter_red_tests PRIVATE uc11_entity uc11_boundary uc11_control uc11_data
    Catch2::Catch2WithMain)
uc11_enable_coverage(unit_converter_red_tests)
target_compile_definitions(unit_converter_red_tests PRIVATE UC11_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

include(CTest)
include(Catch)
catch_discover_tests(unit_converter_tests)
catch_discover_tests(unit_converter_red_tests)

# Golden Master (REG-04): explicit ctest entry for CI / README GM-06
add_test(
    NAME GoldenMaster
    COMMAND unit_converter_tests "golden_master_cli_stdout_approval"
)
set_tests_properties(GoldenMaster PROPERTIES LABELS "golden;REG-04;integration")
