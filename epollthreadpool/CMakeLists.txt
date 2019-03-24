cmake_minimum_required(VERSION 3.13)
project(epollthreadpool)

set(CMAKE_CXX_STANDARD 17)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
#target_link_libraries(epollthreadpool Threads::Threads)

add_executable(epollthreadpool main.cpp lock.h pthreadpool.h scopeguard.h task.h epollmanager.h server.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(epollthreadpool Threads::Threads)