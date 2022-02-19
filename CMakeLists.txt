cmake_minimum_required(VERSION 3.19)
project(COMiC C)

set(CMAKE_C_STANDARD 99)

#find_package(OpenSSL REQUIRED)


set(COMiC_VERSION_MAJOR 0)
set(COMiC_VERSION_MINOR 0)
set(COMiC_VERSION_PATCH 0)
set(COMiC_VERSION_LEVEL a)
set(COMiC_VERSION_SERIAL 0)
set(COMiC_VERSION ${COMiC_VERSION_MAJOR}.${COMiC_VERSION_MINOR}.${COMiC_VERSION_PATCH}${COMiC_VERSION_LEVEL}${COMiC_VERSION_SERIAL})
message(STATUS "Building COMiC ${COMiC_VERSION}")

add_subdirectory(os)

file(
        GLOB
        __COMiC_SOURCES
        LIST_DIRECTORIES false
        ./src/*.c
)

add_library(
        __COMiC_static
        STATIC

        ${__COMiC_SOURCES}
)

add_library(
        __COMiC_shared
        SHARED

        src/empty.c
)

target_link_libraries(__COMiC_static PRIVATE COMiC::os)
target_link_libraries(__COMiC_shared PRIVATE __COMiC_static)
target_include_directories(__COMiC_shared PUBLIC include)

add_library(COMiC ALIAS __COMiC_shared)

