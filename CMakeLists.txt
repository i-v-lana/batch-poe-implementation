cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH HELPER_PARENT)
set(CMAKE_TOOLCHAIN_FILE "${HELPER_PARENT}/include/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
message(CMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}")

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
endif()

project(src)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
pkg_check_modules(cryptopp REQUIRED IMPORTED_TARGET cryptopp)

add_executable(src
        main.cpp
        proof.cpp
        proof.h
        wesolowski.cpp
        wesolowski.h batching.cpp batching.h mpz_helper.cpp mpz_helper.h tests.cpp tests.h NaiveApproach.cpp NaiveApproach.h PRF_crypto.cpp PRF_crypto.h SubsetBatching.cpp SubsetBatching.h HybridBatching.cpp HybridBatching.h BucketBatching.cpp BucketBatching.h GenInstances.cpp GenInstances.h helper.cpp helper.h Experiments.cpp Experiments.h)

target_link_libraries(src PkgConfig::gmp)
target_link_libraries(src PkgConfig::gmpxx)
target_link_libraries(src PkgConfig::cryptopp)