cmake_minimum_required(VERSION 3.10)
project(Edmonds-Karp-Algorithm)

set(CMAKE_CXX_FLAGS "-Wall")

set (input01 "test_graph.txt")
set (input02 "")
set (input03 "")
foreach(file IN LISTS input01 input02 input03)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/utils)
add_executable(EdmondsKarp maximum_flow_algorithms.cpp main.cpp)