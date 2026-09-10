cmake_minimum_required(VERSION 3.20)

project(FunBox
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core Library

add_library(funbox
    src/core/Clock.cpp
    src/snake/SnakeGame.cpp
)

target_include_directories(funbox
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
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
    tests/core/ClockTest.cpp
    tests/snake/SnakeGameTest.cpp
)

target_include_directories(funbox_tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(funbox_tests
    PRIVATE funbox GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(funbox_tests)