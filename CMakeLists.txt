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


file(
        GLOB_RECURSE
        __COMiC_SOURCES
        LIST_DIRECTORIES false
        ./src/*.c
)

add_library(
        __COMiC
        SHARED

        src/empty.c
        #        ${__COMiC_OS_SOURCES}
        #        ${__COMiC_SOURCES}
)

target_include_directories(__COMiC PUBLIC ./include)

add_subdirectory(os)



install(
        TARGETS __COMiC
        RUNTIME DESTINATION ./bin
        PUBLIC_HEADER DESTINATION ./include
)

add_library(COMiC ALIAS __COMiC)