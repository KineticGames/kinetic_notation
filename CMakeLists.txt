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

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/flex_bison)

flex_target(
    lexer
    flex-bison/scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/flex_bison/scanner.auto.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/flex_bison/scanner.auto.h
)

bison_target(
    parser
    flex-bison/specification.y
    ${CMAKE_CURRENT_BINARY_DIR}/flex_bison/parser.auto.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/flex_bison/parser.auto.h
    COMPILE_FLAGS "-Wall"
)

add_flex_bison_dependency(lexer parser)

add_library(kinetic_notation STATIC
    # Source files
    src/ir_token_info.c
    src/kinetic_notation.c

    # Private header files
    src/api_internal.h

    # Public header files
    include/ir_token_info.h
    include/object_list.h
    include/key_type.h

    # Autogen
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)

target_include_directories(kinetic_notation 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/flex_bison
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

add_executable(kinetic_notation_unit_test
    test/parser_test.cpp
)

target_link_libraries(kinetic_notation_unit_test
    gtest_main
    kinetic_notation
)

include(GoogleTest)
gtest_discover_tests(kinetic_notation_unit_test)

add_executable(kinetic_notation_integration_test
    test/kinetic_notation_test.c
)

target_link_libraries(kinetic_notation_integration_test
    kinetic_notation
)

add_test(NAME kinetic_notation_integration_test COMMAND kinetic_notation_integration_test)

