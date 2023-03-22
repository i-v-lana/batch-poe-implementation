cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
find_library(GMP HINTS "/usr/local/Cellar/gmp/6.2.1")

add_executable(src
        main.cpp
        proof.cpp
        proof.h
        wesolowski.cpp
        wesolowski.h)
target_link_libraries(src "/usr/local/Cellar/gmp/6.2.1")
