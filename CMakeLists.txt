cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
#add_compile_options("LDFLAGS=-L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++")
#link_libraries("LDFLAGS=-L/usr/local/opt/llvm/lib/c++ -Wl,-rpath,/usr/local/opt/llvm/lib/c++")
#set(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/12.2.0/bin/gcc-12")
#set(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/12.2.0/bin/g++-12")

set(CMAKE_C_COMPILER "/usr/local/Cellar/llvm/16.0.1/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/local/Cellar/llvm/16.0.1/bin/clang++")

#find_library(GMP HINTS "/usr/local/Cellar/gmp/6.2.1_1")

add_executable(src
        main.cpp
        proof.cpp
        proof.h
        wesolowski.cpp
        wesolowski.h)
#target_link_libraries(src "/usr/local/Cellar/gmp/6.2.1_1")
include_directories("/usr/local/include")
include_directories("/usr/local/lib")

target_link_libraries(src "/usr/local/lib/libgmp.a")
target_link_libraries(src "/usr/local/lib/libgmpxx.a")
