cmake_minimum_required(VERSION 3.0)
project(smartmutex)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(smartmutex main.cpp acyclic_graph.cpp acyclic_graph.h smart_mutex.cpp smart_mutex.h)
