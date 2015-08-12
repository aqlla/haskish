cmake_minimum_required(VERSION 3.2)
project(haskish)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

set(SOURCE_FILES main.cpp src/data_types.h tests/tests.h src/static_list.h src/firstclass.h)
add_executable(haskish ${SOURCE_FILES})