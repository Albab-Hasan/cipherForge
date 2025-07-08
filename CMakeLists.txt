cmake_minimum_required(VERSION 3.20)

project(cipherforge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile commands for clang-tidy/clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add source directories
add_subdirectory(src)
add_subdirectory(tests)

# Enable testing
include(CTest)
add_test(NAME all_tests COMMAND cipherforge_tests)
