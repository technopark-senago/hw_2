cmake_minimum_required(VERSION 3.18)

project(scanner)

option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)
option(ENABLE_COVERAGE "Enable coverage" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(PARALLEL "Enable parallel solution" OFF)

add_compile_options(-Wpedantic -Werror -Wall -Wextra -Wshadow)

if(ENABLE_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
endif()

if(ENABLE_CPPCHECK)
  include(cmake/cppcheck.cmake)
endif()

if(ENABLE_COVERAGE)
  add_compile_options(--coverage -g -O0)
  add_link_options(--coverage)
endif()

if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=leak)
  add_compile_options(-fsanitize=undefined)
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
  add_link_options(-fsanitize=leak)
  add_link_options(-fsanitize=undefined)
  add_link_options(-fsanitize=address)
elseif(NOT ENABLE_COVERAGE)
  add_compile_options("-O3")
endif()

if(PARALLEL)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

add_subdirectory(lib/heap)
add_subdirectory(lib/utils)
add_subdirectory(lib/errors)
add_subdirectory(lib/fscanner)
add_subdirectory(lib/dscanner)
add_subdirectory(lib/registry)
add_subdirectory(lib/constants)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} dscanner)

add_executable(benchmark benchmark/main.c)
target_link_libraries(benchmark dscanner)
