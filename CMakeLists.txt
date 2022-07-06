cmake_minimum_required(VERSION 3.00)
project(COMiC C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

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


add_library(
        core
        STATIC

        $<TARGET_OBJECTS:COMiC::OS>

        # ./src/application.c
        # src/error.c
        # src/error.cpp
        # ./src/single_buffer.c
        # ./src/global_heap.c
        # ./src/rb_tree.c
        # ./src/rb_tree_pointers.cpp
        # ./src/chunk_extra_buffer.h
        # ./src/chunk_extra_buffer.c
        # ./src/chunk_extra_buffer.cpp
        # ./src/validate_version.cpp
        # ./src/validate_type_sizes.cpp
        src/sentinel.cpp
)


set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
add_subdirectory(os)
target_include_directories(core PUBLIC ./include $<TARGET_PROPERTY:COMiC::OS,INCLUDE_DIRECTORIES>)

add_library(COMiC::Core ALIAS core)

install(
        TARGETS core
)

install(
        DIRECTORY ./include
        DESTINATION ./
)