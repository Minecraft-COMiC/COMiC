cmake_minimum_required(VERSION 3.00)
project(COMiC CXX)

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

        src/sentinel.cpp

        src/util/uuid.cpp
        src/util/md5.cpp
        src/util/sha1.cpp
        src/util/aes.cpp

        src/network/buffer.cpp
        src/network/sender.cpp
        src/network/receiver.cpp
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

#####################################################################################

if (WIN32)
    set(OS_DEPENDENT_LIBRARIES ws2_32 Wininet)
elseif (UNIX AND NOT APPLE)
    find_package(CURL REQUIRED)
    set(OS_DEPENDENT_LIBRARIES curl)
elseif (APPLE)
    set(OS_DEPENDENT_LIBRARIES)
endif ()

add_executable(
        run
        run.cpp
)

target_link_libraries(run COMiC::Core ${OS_DEPENDENT_LIBRARIES})
