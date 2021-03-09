cmake_minimum_required(VERSION 3.1)
project(infinitetrade)

set(CMAKE_CXX_STANDARD 11)

add_executable(infinitetrade main.cpp vertex.cpp graph.cpp infinitetrade.cpp test.cpp)