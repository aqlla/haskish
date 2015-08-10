cmake_minimum_required(VERSION 3.2)
project(haskish)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp src/data_types.h src/meta.h tests/tests.h src/static_list.h)
add_executable(haskish ${SOURCE_FILES})