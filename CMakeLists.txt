cmake_minimum_required(VERSION 3.10)

project(kuro)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(KURO_BUILD_TESTS "build tests" ON)
option(KURO_BUILD_EXAMPLES "build examples" ON)

if (KURO_BUILD_TESTS)
    message(STATUS "Kuro Build Tests Enabled")
    add_subdirectory(external/doctest EXCLUDE_FROM_ALL)
    add_subdirectory(utests)
endif(KURO_BUILD_TESTS)

if (KURO_BUILD_EXAMPLES)
    message(STATUS "Kuro Build Examples Enabled")
    add_subdirectory(examples)
endif(KURO_BUILD_EXAMPLES)

add_library(kuro INTERFACE)
target_include_directories(kuro INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/kuro)