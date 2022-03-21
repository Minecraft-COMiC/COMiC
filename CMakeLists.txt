cmake_minimum_required(VERSION 3.19)
project(COMiC C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(COMiC_VERSION_MAJOR 0)
set(COMiC_VERSION_MINOR 0)
set(COMiC_VERSION_MICRO 0)
set(COMiC_VERSION_LEVEL a)
set(COMiC_VERSION_SERIAL 1)
set(COMiC_VERSION ${COMiC_VERSION_MAJOR}.${COMiC_VERSION_MINOR}.${COMiC_VERSION_MICRO}${COMiC_VERSION_LEVEL}${COMiC_VERSION_SERIAL})

message(STATUS "Building COMiC ${COMiC_VERSION}")

add_library(
        __COMiC_Headers
        INTERFACE
)
target_include_directories(__COMiC_Headers INTERFACE ./include)


add_library(COMiC::Headers ALIAS __COMiC_Headers)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_library(
        core
        STATIC

        ./src/application.c
        ./src/error.c
        ./src/version.cpp

        ./src/network/buffer.c
        ./src/network/buffer.h
        ./src/network/packet/receiver.c
        ./src/network/packet/receiver.h
        ./src/network/packet/sender.c
        ./src/network/packet/sender.h

        ./src/util/juuid/juuid.c
        ./src/util/juuid/juuid.h
        ./src/util/rsa/rsa.c
        ./src/util/rsa/rsa.h

        $<TARGET_OBJECTS:COMiC::OS>
)


add_subdirectory(os)

target_include_directories(core PUBLIC ./include $<TARGET_PROPERTY:COMiC::OS,INCLUDE_DIRECTORIES>)
target_link_libraries(core OpenSSL::Crypto)

add_library(COMiC::Core ALIAS core)

install(
        TARGETS core
)

install(
        DIRECTORY ./include
        DESTINATION ./
)


#####################################################################################

add_executable(
        run
        run.c
)

target_link_libraries(run COMiC::Core)
