cmake_minimum_required(VERSION 3.10)

# Enable group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

project(kinetic_notation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# ====================================
# GTEST

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set_property(TARGET gmock PROPERTY FOLDER "gtest")
set_property(TARGET gmock_main PROPERTY FOLDER "gtest")
set_property(TARGET gtest PROPERTY FOLDER "gtest")
set_property(TARGET gtest_main PROPERTY FOLDER "gtest")

# ====================================
# FLEX BISON

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(
    lexer
    flex-bison/scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.auto.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/scanner.auto.h
)

bison_target(
    parser
    flex-bison/specification.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.auto.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.auto.h
)

add_flex_bison_dependency(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

add_library(kinetic_notation STATIC
    # Source files
    src/token_info.c

    # Header files
    include/token_info.h

    # Autogen
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)

enable_testing()

add_executable(kinetic_notation_test
    test/compilation_test.cpp
)

target_link_libraries(kinetic_notation_test
    gtest_main
    kinetic_notation
)

include(GoogleTest)
gtest_discover_tests(kinetic_notation_test)

