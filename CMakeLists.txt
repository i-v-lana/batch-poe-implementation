cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

set(CMAKE_C_COMPILER "/usr/local/Cellar/llvm/16.0.1/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/local/Cellar/llvm/16.0.1/bin/clang++")

add_executable(src
        main.cpp
        proof.cpp
        proof.h
        wesolowski.cpp
        wesolowski.h batching.cpp batching.h)

include_directories("/usr/local/include")
include_directories("/usr/local/lib")

target_link_libraries(src "/usr/local/lib/libgmp.a")
target_link_libraries(src "/usr/local/lib/libgmpxx.a")
