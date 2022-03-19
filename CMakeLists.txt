cmake_minimum_required(VERSION 3.19)
project(COMiC C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(COMiC_VERSION_MAJOR 0)
set(COMiC_VERSION_MINOR 0)
set(COMiC_VERSION_PATCH 0)
set(COMiC_VERSION_LEVEL a)
set(COMiC_VERSION_SERIAL 0)
set(COMiC_VERSION ${COMiC_VERSION_MAJOR}.${COMiC_VERSION_MINOR}.${COMiC_VERSION_PATCH}${COMiC_VERSION_LEVEL}${COMiC_VERSION_SERIAL})

message(STATUS "Building COMiC ${COMiC_VERSION}")

add_subdirectory(libs/util)
add_subdirectory(libs/os)
add_subdirectory(libs/core)

add_dependencies(__COMiC_Core COMiC::OS)