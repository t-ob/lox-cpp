cmake_minimum_required(VERSION 3.25)

project(lox_cpp_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/adc514538678a61b13c240f7b41babbc03b2ac24.zip
)
FetchContent_MakeAvailable(googletest)

set(TEST_SRC
        main.cpp)

add_executable(run_lox_cpp_tests ${TEST_SRC})

# Link the test executable with the Google Test main entry point and your own test library
target_link_libraries(run_lox_cpp_tests gtest gtest_main lox_cpp)

target_include_directories(run_lox_cpp_tests PRIVATE ../src)

# Enable testing functionality
enable_testing()

add_test(NAME run_lox_cpp_tests COMMAND run_lox_cpp_tests)
