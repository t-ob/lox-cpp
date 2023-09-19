cmake_minimum_required(VERSION 3.25)
project(lox_cpp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

add_executable(lox bin/main.cpp)
target_link_libraries(lox lox_cpp)
