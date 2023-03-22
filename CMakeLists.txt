cmake_minimum_required(VERSION 3.10)

project(kinetic_notation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -ggdb")

add_library(
  kinetic_notation STATIC
  # Source files
  src/definition.c
  src/error.c
  src/hashmap/hashmap.c
  src/hashmap/bucket.c
  src/hashmap/bucket.h
  src/parser.c
  src/scanner.c
  # Private header files
  src/error.h
  src/hashmap.h
  src/parser.h
  src/scanner.h
  # Public header files
  include/kinetic_notation.h
  include/kinetic_notation/definition.h
  include/kinetic_notation/types.h)

target_include_directories(
  kinetic_notation
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)
