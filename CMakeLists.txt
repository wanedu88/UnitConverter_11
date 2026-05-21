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

add_library(uc11_entity
    src/entity/UnitRegistry.cpp
    src/entity/UnitConverter.cpp
)
target_include_directories(uc11_entity PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(uc11_boundary
    src/boundary/InputParser.cpp
    src/boundary/InputValidator.cpp
    src/boundary/ConfigLoader.cpp
    src/boundary/OutputFormatter.cpp
)
target_include_directories(uc11_boundary PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(uc11_boundary PUBLIC uc11_entity)

add_executable(unit_converter_legacy UnitConverter.cpp)

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
)
target_link_libraries(unit_converter_tests PRIVATE uc11_entity uc11_boundary Catch2::Catch2WithMain)
target_compile_definitions(unit_converter_tests PRIVATE UC11_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(unit_converter_red_tests
    tests/red_track_a_boundary.cpp
    tests/red_track_b_domain.cpp
)
target_link_libraries(unit_converter_red_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(unit_converter_tests)
catch_discover_tests(unit_converter_red_tests)
