cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH HELPER_PARENT)
set(CMAKE_TOOLCHAIN_FILE "${HELPER_PARENT}/include/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
message(CMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}")

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")

    #uncomment?: include_directories("/usr/local/include")
    #uncomment?: include_directories("/usr/local/lib")
endif()
if(WIN32)

#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp -lgmpxx")
#    message(CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}")
#    file(GLOB SUBDIRECTORIES "${HELPER_PARENT}/include/vcpkg/downloads/tools/clang/clang*")
#
#    # Iterate through the found directories and extract the first match
#    list(LENGTH SUBDIRECTORIES NUM_SUBDIRECTORIES)
#    if(NUM_SUBDIRECTORIES GREATER 0)
#        list(GET SUBDIRECTORIES 0 FIRST_SUBDIRECTORY)
#        # Store the result in a variable
#        set(CLANG_SUBDIRECTORY ${FIRST_SUBDIRECTORY})
#    else()
#        message(FATAL_ERROR "No subdirectory starting with 'clang' found.")
#    endif()
#    set(CMAKE_C_COMPILER "${CLANG_SUBDIRECTORY}/bin/clang.exe")
#    set(CMAKE_CXX_COMPILER "${CLANG_SUBDIRECTORY}/bin/clang++.exe")
#    message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
#    message(CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")
endif()

project(src)

set(CMAKE_CXX_STANDARD 14)


#include_directories("../include")
#include_directories("C:/Users/JanHruby/Desktop/lanochka_rocnikac/include/vcpkg/installed/x64-windows/include/cryptopp")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

#UNCOMNMENT: set(CMAKE_C_COMPILER "clang")

#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#UNCOMMENT: set(CMAKE_CXX_COMPILER "clang++")



#target_link_libraries(src "/usr/local/lib/libgmp.a")
#target_link_libraries(src "/usr/local/lib/libgmpxx.a")

#find_package(OpenSSL REQUIRED)
#target_link_libraries(src PRIVATE OpenSSL::SSL OpenSSL::Crypto)

#  gmp
#set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
pkg_check_modules(cryptopp REQUIRED IMPORTED_TARGET cryptopp)

message(include_dir="${gmp_INCLUDE_DIRS}")
#UN1: cmake_path(GET gmp_INCLUDE_DIRS PARENT_PATH ALL_INCLUDE)
#UN1: link_directories("${cryptopp_LIBRARY_DIRS}")
#
#UN1: link_directories("${ALL_INCLUDE}/lib")
#UN1: message(linked_directories="${ALL_INCLUDE}/lib")

add_executable(src
        main.cpp
        proof.cpp
        proof.h
        wesolowski.cpp
        wesolowski.h batching.cpp batching.h mpz_helper.cpp mpz_helper.h tests.cpp tests.h NaiveApproach.cpp NaiveApproach.h PRF_crypto.cpp PRF_crypto.h SubsetBatching.cpp SubsetBatching.h HybridBatching.cpp HybridBatching.h BucketBatching.cpp BucketBatching.h GenInstances.cpp GenInstances.h helper.cpp helper.h Experiments.cpp Experiments.h)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L C:/Users/JanHruby/Desktop/lanochka_rocnikac/include/vcpkg/installed/x64-windows/lib -lcryptopp")
#UN2: target_include_directories(src PUBLIC "${gmp_INCLUDE_DIRS}")
#UN2: target_include_directories(src PUBLIC "${gmp_INCLUDE_DIRS}/cryptopp")
#UN2: target_include_directories(src PUBLIC "${cryptopp_INCLUDE_DIRS}")
target_link_libraries(src PkgConfig::gmp)

# gmpxx
#find_package(PkgConfig REQUIRED)
target_link_libraries(src PkgConfig::gmpxx)
target_link_libraries(src PkgConfig::cryptopp)
#find_package(cryptopp CONFIG REQUIRED)
#target_link_libraries(src cryptopp::cryptopp)


#target_link_libraries(src PkgConfig::cryptopp)
#target_link_libraries(src gmp)
#target_link_libraries(src gmpxx)

#target_link_libraries(src PRIVATE /usr/local/lib/libgmp.a /usr/local/lib/libgmpxx.a)
#target_link_libraries(src PRIVATE /usr/local/lib/crypto++/libcryptopp.a)