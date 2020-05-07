cmake_minimum_required(VERSION 3.10)
project(Edmonds-Karp-Algorithm)

set(CMAKE_CXX_FLAGS "-Wall")

set (input01 "graph1.txt")
set (input02 "graph2.txt")
set (input03 "graph3.txt")
set (input04 "graph4.txt")
set (input05 "graph5.txt")
foreach(file IN LISTS input01 input02 input03 input04 input05)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/utils)
add_executable(EdmondsKarp main.cpp)