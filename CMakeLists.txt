cmake_minimum_required(VERSION 3.20)

project(FunBox
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core Library

add_library(funbox
    src/core/Clock.cpp
)

target_include_directories(funbox
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# GoogleTest

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()

# Tests

add_executable(funbox_tests
    tests/ClockTest.cpp
)

target_include_directories(funbox_tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(funbox_tests
    PRIVATE funbox GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(funbox_tests)