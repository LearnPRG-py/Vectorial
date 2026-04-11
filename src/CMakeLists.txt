cmake_minimum_required(VERSION 3.15)
project(Vectorial)

set(CMAKE_CXX_STANDARD 11)

add_executable(test testing/test.cpp)
target_include_directories(test PRIVATE src)
