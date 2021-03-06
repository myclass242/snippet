cmake_minimum_required(VERSION 3.13)
project(infinitetrade)

set(CMAKE_CXX_STANDARD 11)

add_executable(infinitetrade main.cpp vertex.cpp graph.cpp infinitetrade.cpp test.cpp)