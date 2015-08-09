cmake_minimum_required(VERSION 3.2)
project(haskish)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp data_types.h meta.h)
add_executable(haskish ${SOURCE_FILES})