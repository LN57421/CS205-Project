cmake_minimum_required(VERSION 3.23)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(calculator main.cpp calc.cpp)
add_executable(calculator_calc main.cpp calc.cpp)

add_library(test_library STATIC calc.cpp)
