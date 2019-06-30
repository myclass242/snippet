cmake_minimum_required(VERSION 3.10)
project(s06)

#SET(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 11)

set(SRC
    Eventloop.h
    Eventloop.cpp
    Poller.h
    Poller.cpp
    Channel.h
    Channel.cpp
    Timer.h
    Timer.cpp
    TimerId.h
    TimerQueue.h
    TimerQueue.cpp
    EventLoopThread.h
    EventLoopThread.cpp
    SockOps.h
    SockOps.cpp
    InetAddress.h
    InetAddress.cpp
    Socket.h
    Socket.cpp
    Acceptor.h
    Acceptor.cpp
    TcpServer.h
    TcpServer.cpp
    TcpConnection.h
    TcpConnection.cpp)

add_subdirectory(base)

add_executable(test1 ${SRC} test1.cpp)
target_link_libraries(test1 muduo_base)

add_executable(test2 ${SRC} test2.cpp)
target_link_libraries(test2 muduo_base)

add_executable(test3 ${SRC} test3.cpp)
target_link_libraries(test3 muduo_base)

add_executable(test4 ${SRC} test4.cpp)
target_link_libraries(test4 muduo_base)

add_executable(test5 ${SRC} test5.cpp)
target_link_libraries(test5 muduo_base)

add_executable(test6 ${SRC} test6.cpp)
target_link_libraries(test6 muduo_base)

add_executable(test7 ${SRC} test7.cpp)
target_link_libraries(test7 muduo_base)

add_executable(test8 ${SRC} test8.cpp)
target_link_libraries(test8 muduo_base)