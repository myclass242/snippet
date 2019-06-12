cmake_minimum_required(VERSION 3.10)
project(s01)

set(CMAKE_CXX_STANDARD 11)

set(SRC
    Eventloop.h
    Eventloop.cpp
    Poller.h
    Poller.cpp
    Channel.h
    Channel.cpp)

add_subdirectory(base)

add_executable(test1 ${SRC} test1.cpp)
target_link_libraries(test1 muduo_base)

add_executable(test2 ${SRC} test2.cpp)
target_link_libraries(test2 muduo_base)

add_executable(test3 ${SRC} test3.cpp)
target_link_libraries(test3 muduo_base)
