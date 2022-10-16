cmake_minimum_required(VERSION 3.10)

project(Project2)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(./calculator DIR_SRCS)

include_directories(calculator_h)

#add_executable(calculator_V2 ${DIR_SRCS})


add_executable(cal ${DIR_SRCS})