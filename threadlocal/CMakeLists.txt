cmake_minimum_required(VERSION 3.14)
project(threadlocal)

set(CMAKE_CXX_STANDARD 11)

add_executable(threadlocal main.cpp CurrentThread.h CurrentThread.cpp)

target_link_libraries(threadlocal pthread)